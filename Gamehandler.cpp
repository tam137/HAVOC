#ifndef GAMEHANDLER_CPP_INCLUDED
#define GAMEHANDLER_CPP_INCLUDED

#include "Gamehandler.h"

#define USEASSERTS  0
#define USETHREADS  1

/**
 * Returns the evalfrom last real Move, 0 if none
 */
int Gamehandler::getLastEval() {
    if (this->moveResults.size() > 0) {
        return this->moveResults[this->moveResults.size()-1].getBestMove().getEval();
    } else {
        return 0;
    }        
}

/**
 * Return the calc time for the move in seconds
 */
long int Gamehandler::getMoveCalcTime() {
    long int curTime = std::clock();
    return ((curTime - this->startTimeInt) / 1000000);
}



Gamehandler::Gamehandler () { }

Gamehandler::Gamehandler (EngineOpt engineOpt) {
    this->board = Board(engineOpt.whitePlayer.evalOpt, engineOpt.blackPlayer.evalOpt);
    this->engineOpt = engineOpt;
    this->pgnHeader = engineOpt.pgnHeader;
    this->treeOpt = engineOpt.treeOpt;
    return;
}

Gamehandler::~Gamehandler () {
    delete this->lastTree;
    delete hash;
}

/**
 * do a Move on a gh instance Board
 */
void Gamehandler::doMove(const CalcMoveResult &result) {
    Move move = result.getBestMove();
    moveResults.push_back(result);
    #if USEASSERTS == 1
    assert (move.getPre() == NULL);
    #endif
    this->board = *(board.returnBoardForMoves(&move));
    this->hash->addBoardFreq(this->board);
    
}

/**
 * Creates MoveResults and do all moves on a gh instance Board
 * 2nd param, list starts with white
 */
void Gamehandler::doMoveList(std::vector<std::string> moveList, bool white) {
    CalcMoveResult result;
    for (std::string moveStr : moveList) {
        Move move = Move(moveStr, white);
        result.setBestMove(move);
        result.setMoveNr(this->getMoveResults().size() +1);
        this->doMove(result);
    }
}



/********************************************
 * The main loop to calculate a Move        *
 *                                          *
 *                                          *
 *******************************************/

/**
 * Calculate the best move with the given Engine Opt
 */
CalcMoveResult Gamehandler::calcMove() {

    bool bWhite = 1 - (this->getMoveCount() % 2);
    std::clock_t startTime = std::clock();
    this->prioMoveList.clear();
    this->startTimeInt = startTime;
    std::vector<Move> bestMoveL;
    CalcMoveResult res;
    int initEval = 0;
    if (!this->moveResults.empty()) {
        initEval = moveResults.at(moveResults.size()-1).getBestMove().getEval();
    }        

    int adjMaxCalcLvl = treeOpt.maxcalcLvl;

    if (engineOpt.useVariance) {
        adjMaxCalcLvl -= helper::getRandom(2);
    }

    // level drauf wenn Complexität gering
    if (this->board.getComplexity() <= engineOpt.complexityboarder) {
        adjMaxCalcLvl += 2;
    }
    

    Tree *tree = new Tree();

    int startLvl = 1;
    if (!treeOpt.useIterateDeepening) {
        startLvl = treeOpt.maxcalcLvl;
    }

    for (int localLvl = startLvl; localLvl <= adjMaxCalcLvl; localLvl++) {
        // max calc level for hit moves in alpha beta
        int localHitLvl = treeOpt.maxcalcLvlHit - (adjMaxCalcLvl - localLvl);
        if (this->calcTimeMs() > treeOpt.maxCalcTime*100 || localLvl > adjMaxCalcLvl) {
            break;
        }
        tree = NULL;
        delete tree;
        tree = new Tree(board, bWhite, engineOpt, this->prioMoveList, initEval, this->hash);
        tree->alphaBetaExp(this->board, 1, localLvl, localHitLvl, NULL, -100000, +100000, bWhite);
        this->lastTree = tree;
        auto bestMoveRow = this->getBestMoveRow(tree->getBestMove());
        
        for (Move m : bestMoveRow) {
            this->addMoveToPrioList(m, 9);
        }                    

        res.setBestMove(tree->getBestMove());
        res.setBestMoveRow(getBestMoveRow(tree->getBestMove()));
        res.setCalcTime(std::clock() - startTime);
        res.setCreatedNodesCount(tree->getMinMaxCreatedNodes());
        res.setMoveNr(this->moveResults.size() + 1);
        this->lastTree = tree;
        res.setLastTree(tree);
        #if USETHREADS == 1
        if (engineOpt.uciMode) {
            std::cout << "info score cp " << (bWhite ? res.getBestMove().getEval() : -res.getBestMove().getEval())
                    << " depth " << localLvl
                    << " nodes " << res.getCreatetNodes()/1000
                    //<< " time " << 10
                    << " pv "
                    << res.getBestMove().toUciString();
            std::cout << std::endl;
        }
        #endif

        // premature termination
        bestMoveL.push_back(tree->getBestMove());
        bool term = false;
        if (bestMoveL.size() == treeOpt.preTermLvl) {
            for (int i = 1; i < bestMoveL.size(); i++) {
                term = bestMoveL.at(i).isEqal(&(bestMoveL.at(i-1))) ? true : false;
                if (term == false) break;
            }
        }
        if (term) {
            break;
        }
    }

    this->hash->resetHash32();
    delete tree;
    tree = NULL;        
    return res;     
}

int Gamehandler::calcTimeMs() {
    return (std::clock() - this->startTimeInt) / 1000;
}

/**
 * Return best MoveRow after calculat best move
 */
std::vector<Move> Gamehandler::getBestMoveRow(Move move) const {
    std::vector<Move> firstLvl = this->lastTree->getSortedMovesFirstLvl();
    return this->lastTree->getMoveRow(move);
}

/**
 * pgn Export
 */
void Gamehandler::exportPgn(char const filename[]) {
    PgnExport pgn = PgnExport(filename, this->moveResults, this->pgnHeader);
    pgn.writeGameToPgn();
}

/**
 * Returns the Board of the Gamehandler
 */
Board Gamehandler::getBoard() {
    return this->board;
}

/**
 * Returns a std::vector<CalcMoveResult> with all played Moves
 */
std::vector<CalcMoveResult> Gamehandler::getMoveResults() {
    return this->moveResults;
}

/**
 * return the engine options
 */
EngineOpt Gamehandler::getEngineOpt() {
    return this->engineOpt;
}

/**
 * Return movecount for the game
 */
int Gamehandler::getMoveCount() {
    return this->moveResults.size();
}

/**
 * Returns the last evaluated tree
 */
Tree *Gamehandler::getLastTree() {
    Tree *ret = this->lastTree;
    return ret;
}

/**
 * Insert a Move to prio hashmap if not exists
 * or sets the new prio if exists
 */
void Gamehandler::addMoveToPrioList(Move move, int prio) {
    int hash = move.getMoveHash();
    this->prioMoveList[hash] = prio;
    return;
}



#endif
