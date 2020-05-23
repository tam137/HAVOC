#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "Tree.h"

#define USEASSERTS  0

Tree::Tree() { };

/**
 * Reset stats and delete all nodes from heap
 */
Tree::~Tree () {
    for (int i = 0; i < this->ptrs.size(); i++) {
        delete this->ptrs[i];
        this->ptrs[i] = NULL;
    }
    //stats::reset();
}

/**
 * The Tree constructor
 */
Tree::Tree(Board board, bool white, EngineOpt engineOpt, Hash* hash) {
    this->board = board;
    this->treeOpt = engineOpt.treeOpt;
    this->engineOpt = engineOpt;
    this->white = white;
    this->hash32uses = 0;
    this->prioMoveList = std::unordered_map<int, char>();
    this->hash = hash;
    this->minMaxCreatedNodes = 0;
}


/**
 * The Tree constructor with prio List
 */
Tree::Tree(Board board, bool white, EngineOpt engineOpt, std::unordered_map<int, char> prioMoveList, int initEval, Hash* hash) {
    this->board = board;
    this->treeOpt = engineOpt.treeOpt;
    this->engineOpt = engineOpt;
    this->white = white;
    this->hash32uses = 0;
    this->prioMoveList = prioMoveList;
    this->initEval = initEval;
    this->hash = hash;
    this->minMaxCreatedNodes = 0;
}

/**
 * Alpha Beta for quiescence moves
 */
int Tree::quiescence(Board &board, int level, Move *preMove, int alpha, int beta, bool white) {
    
    EvalOpt evalOpt = this->white ? this->engineOpt.whitePlayer.evalOpt : this->engineOpt.blackPlayer.evalOpt;

    #if USEASSERTS == 1
    assert (preMove->isHit());
    assert (level > treeOpt.maxcalcLvl);
    #endif
    
    Eval *eval = new Eval();
    int evalV = eval->evalMove(&board, EvalModus::deep, NULL, evalOpt);

    if (white) {
        if (evalV >= beta) {
            return evalV;
        }

        if (alpha < evalV) {
            alpha = evalV;
        }
    } else {
        if (evalV <= alpha) {
            return evalV;
        }

        if (beta > evalV) {
            beta = evalV;
        }
    }
    
    Movegen* movegen = new Movegen();
    std::list<Move> *movelist = new std::list<Move>();
    if (level > this->engineOpt.treeOpt.maxcalcLvlHit) {
        //return evalV;
    } else {
        movelist = movegen->getMoveList(board.getBoard(), preMove, white, true);
    }
    
    this->minMaxCreatedNodes += movelist->size();

    Move *postMove = NULL;
    int maxEval = 0;
    int minEval = 0;
    if (white) {
        maxEval = -100000;
    } else {
        minEval = +100000;
    }

    for (auto it = movelist->begin(); it != movelist->end(); it++) {
        Move *movePtr = &(*it);
        preMove->addPostMoveToPostList(movePtr);
        Board *tmpBoard = board.cpyBoard();
        tmpBoard->execSingleMoveOnBoard(movePtr);
        int hash32 = hash->get32BitHash(tmpBoard->getBoard());
        movePtr->setHash32(hash32);
        int eval;
        if (hash->getBoardFreq(hash32) == 2) {
            eval = 0;
        } else {
            eval = quiescence(*tmpBoard, level+1, movePtr, alpha, beta, !white);
        }
        delete tmpBoard;
        tmpBoard = NULL;
        if (white) {
            if (eval > maxEval) {
                postMove = movePtr;
            }
            maxEval = helper::max(maxEval, eval);
            alpha = helper::max(alpha, eval);
        } else {
            if (eval < minEval) {
                postMove = movePtr;
            }
            minEval = helper::min(minEval, eval);
            beta = helper::min(beta, eval);

        }
        if (beta <= alpha) {
            break;
        }
    }
    if (preMove != NULL) {
        preMove->setPost(postMove);
        if (white) {
            preMove->setEval(maxEval);
        } else {
            preMove->setEval(minEval);
        }
    }
    if (white) {
        return maxEval;
    } else {
        return minEval;
    }
}

/**
 * Build and eval the game tree
 * returns tree eval
 */
int Tree::alphaBetaExp(Board &board, int level, int maxLvl, int localHitLvl, Move *preMove, int alpha, int beta, bool white) {

    TreeModus treeModus = TreeModus::full;
    std::list<Move> *movelist;
    EvalOpt evalOpt = this->white ? this->engineOpt.whitePlayer.evalOpt : this->engineOpt.blackPlayer.evalOpt;
    Movegen* movegen = new Movegen();

    if (level > maxLvl) {
        Eval *eval = new Eval();
        if (treeOpt.normalizeModus == NormalizeModus::allHitNodes && level <= treeOpt.maxcalcLvlHit && preMove->isHit()) {
            //int evalV = eval->evalMove(&board, EvalModus::deep, NULL, evalOpt);
            //preMove->setEval(evalV);
            return quiescence(board, level, preMove, alpha, beta, white);
        }

        else {
            int evalV = eval->evalMove(&board, EvalModus::deep, NULL, evalOpt);
            preMove->setEval(evalV);
            return evalV;
        }

        eval = NULL;
        delete eval;
    }

    // get posible moves
    if (treeModus == TreeModus::full) {
        movelist = movegen->getMoveList(board.getBoard(), preMove, white);
    }

    // set move sort prio and hashes
    this->ptrs.push_back(movelist);
    for (auto it = movelist->begin(); it != movelist->end() ; it++) {
        int hash = it->getMoveHash();
        if (this->prioMoveList.count(hash)) {
            it->setPrio(this->prioMoveList[hash]);
        }
        if (it->isHit()) {
            it->addPrio(board.getFigValueAtIndex(it->getTo(), this->white) / 100);
        }
    }
    this->minMaxCreatedNodes += movelist->size();
    movelist->sort(helper::sortPrioDes);
    
    Move *postMove = NULL;
    int maxEval = 0;
    int minEval = 0;
    if (white) {
        maxEval = -100000;
    } else {
        minEval = +100000;
    }
    
    // do main loop
    for (auto it = movelist->begin(); it != movelist->end(); it++) {
        Move *movePtr = &(*it);
        if (level <= treeOpt.checkAllowedMoveLevel && !(board.isAllowedMove(*movePtr))) {
            continue;
        }
        if (preMove == NULL) {
            this->firstLevelNodeList.push_back(movePtr);
        } else {
            preMove->addPostMoveToPostList(movePtr);
        }
        Board *tmpBoard = board.cpyBoard();
        tmpBoard->execSingleMoveOnBoard(movePtr);
        int hash32 = hash->get32BitHash(tmpBoard->getBoard());
        movePtr->setHash32(hash32);
        int eval;
        if (hash->getBoardFreq(hash32) == 2) {
            eval = 0;
        } else {
            eval = alphaBetaExp(*tmpBoard, level+1, maxLvl, localHitLvl, movePtr, alpha, beta, !white);
        }
        delete tmpBoard;
        tmpBoard = NULL;
        if (white) {
            if (eval > maxEval) {
                postMove = movePtr;
            }
            maxEval = helper::max(maxEval, eval);
            alpha = helper::max(alpha, eval);
        } else {
            if (eval < minEval) {
                postMove = movePtr;
            }
            minEval = helper::min(minEval, eval);
            beta = helper::min(beta, eval);

        }
        if (beta <= alpha) {
            break;
        }
    }
    if (preMove != NULL) {
        preMove->setPost(postMove);
        if (white) {
            preMove->setEval(maxEval);
        } else {
            preMove->setEval(minEval);
        }
    }
    if (white) {
        return maxEval;
    } else {
        return minEval;
    }
}


/****************************
 * getter and setter
 */ 

std::vector<Move> Tree::getMoveRow(const Move &move) {
    #if USEASSERTS == 1
    assert(move.getLevel() == 1);
    #endif
    std::vector<Move> moveRow;
    Move buf = move;
    moveRow.push_back(buf);
    while (buf.hasPost()) {
        moveRow.push_back(*(buf.getPostMove()));
        buf = *(buf.getPostMove());
    };
    return moveRow;        
}

/**
 * return a deep sorted Copy of the first Node level
 */
std::vector<Move> Tree::getSortedMovesFirstLvl() {
    std::list<Move> bufList;
    std::vector<Move> bufVec;
    #if USEASSERTS == 1
    assert (!firstLevelNodeList.empty());
    #endif
    for (int i = 0; i < this->firstLevelNodeList.size(); i++) {
        bufList.push_back(*(this->firstLevelNodeList[i]));
    }

    if (this->white)
        bufList.sort(helper::sortEvalDes);
    else
        bufList.sort(helper::sortEvalAsc);
    #if USEASSERTS == 1
    assert (bufList.size() > 0);
    #endif

    return std::vector<Move> (bufList.begin(), bufList.end());
}

/**
 * Returns a copy of best move from alpha beta alg
 */
Move Tree::getBestMove() {
    return getSortedMovesFirstLvl().at(0);
}

std::list<Move*> Tree::getMovesFirstLvlPtr() {
    std::list<Move*> buf;
    for (std::list<Move>::iterator it = this->nodeList.begin(); it->getLevel() == 1; it++) {
        buf.push_back(&(*it));
    }
    return buf;
}

std::vector<long> Tree::getMovesFirstLvlMoveId() {
    std::vector<long> buf;
    for (std::list<Move>::iterator it = this->nodeList.begin(); it->getLevel() == 1; it++) {
        buf.push_back(it->getMoveId());
    }
    return buf;
}

bool Tree::hasNodeForMoveId(int moveId) {
    if (this->mapMoveNr.count(moveId)) {
        #if USEASSERTS == 1
        assert (this->mapMoveNr.count(moveId) == 1);
        #endif
        return true;
    } else {
        return false;
    }        
}

TreeOpt Tree::getTreeOpt() {
    return this->treeOpt;
}

std::list<Move> Tree::getNodeList() {
    return this->nodeList;
}

int Tree::getMoveCount() const {
    return this->nodeList.size();
}

const Board Tree::getBoard() {
    return this->board;
}

int Tree::getEvalFromBestMove() {
    return this->getSortedMovesFirstLvl().begin()->getEval();
}

int Tree::getHash32UsesCount() {
    return this->hash32uses;
}

int Tree::getMinMaxCreatedNodes() {
    return this->minMaxCreatedNodes;
}

void Tree::resetMinMaxCreatedNodes() {

}


#endif