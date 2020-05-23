#ifndef DEBUGCMD_H_INCLUDED
#define DEBUGCMD_H_INCLUDED


#if USEPOSTGRES == 1
#include "Postgres.h"
#endif

class DebugCmd {
private:
    std::string cmd;
    Board board;
    CalcMoveResult calcResult;
    Gamehandler* gh;

public:

    DebugCmd (std::string cmd, Gamehandler* gh, CalcMoveResult calcResult) {
        this->cmd = cmd;
        this->board = gh->getBoard();
        this->calcResult = calcResult;
        this->gh = gh;
    }

    void runCmd() {
        
        /**
         * Show help
         */
        if (this->cmd.compare("_h") == 0) {
            std::cout << "_b\t" << "print Board" << std::endl;
            std::cout << "_p\t" << "print Performance stats" << std::endl;
            std::cout << "_m\t" << "print Moves in game" << std::endl;
            std::cout << "_d\t" << "write in Database" << std::endl;
            std::cout << "_v\t" << "print Variants" << std::endl;
            std::cout << "_l\t" << "print log of last move" << std::endl;
            std::cout << "_pgn\t" << "export as pgn" << std::endl;
        }

        /**
         * show Board
         */
        if (this->cmd.compare("_b") == 0) {
            this->board.printBoard();
            return;
        }


        /**
         * Performance info
         */
        if (this->cmd.compare("_p") == 0) {
            std::list<Move> moveList = this->calcResult.getLastTree()->getNodeList();
            int i = 0;
            unsigned long int treeBytes = sizeof(this->calcResult.getBestMove()) * this->calcResult.getCreatetNodes();
            
            std::cout << "Nodesize (bytes) " << sizeof(this->calcResult.getBestMove()) << std::endl;
            std::cout << "Treesize (Count) " << this->calcResult.getCreatetNodes() << "\t";
            std::cout << "Treesize (mb) " << treeBytes/1024/1024 << std::endl;
            std::cout << "Nodesize (bytes) " << treeBytes << std::endl;
            std::cout << "Moves per second " << this->calcResult.getNodePerMs() << "K" << std::endl;
            std::cout << "Uses last move " << gh->getLastTree()->getHash32UsesCount()/1000 << "K\t" << std::endl;
            std::cout << "EvalSum in game " << 1/1000 << "K" << std::endl;
            return;
        }

        /**
         * Show all Moves
         */
        if (this->cmd.compare("_m") == 0) {
            std::vector<CalcMoveResult> moveList = this->gh->getMoveResults();
            for (int i = 0; i < moveList.size(); i = i + 2) {
                std::cout << " " << (i/2) + 1 << ".\t";
                std::cout << moveList[i].getBestMove().getPgnNot();
                if (i + 1 < moveList.size()) {
                    std::cout << "\t" << moveList[i+1].getBestMove().getPgnNot() << std::endl;
                } else { std::cout << std::endl; }
            }
            return;
        }

        /**
         * Write Tree in database
         */
        if (this->cmd.compare("_d") == 0) {
            #if USEPOSTGRES == 1
            Postgres db = Postgres(getConnection_cop());
            db.writeTree(*(gh->getLastTree()));
            db.writeGameStats(calcResult);
            #else
            std::cout << "no postgres" << std::endl;
            #endif
            return;
        }

        /**
         * print cpu variants
         */
        if (this->cmd.compare("_v") == 0) {
            Tree* tree = this->calcResult.getLastTree();
            std::vector<Move> variants = tree->getSortedMovesFirstLvl();
            assert (false); // not impl
            /*
            for (std::list<Move>::iterator it = variants.begin(); it != variants.end(); it++) {
                it->getEval() >= 0 ? printf ("+") : printf ("");
                printf ("%.2f", it->getEvalD());
                std::vector<Move*> moveRow = it->getMoveRow();
                for (int i = 0; i < moveRow.size(); i++) {
                    std::cout << "\t" << moveRow[i]->getPgnNot();
                }
                std::cout << std::endl;          
            }
            */
            return;
        }

        /**
         * prints the move log
         */
        if (this->cmd.compare("_l") == 0) {
            #if USEASERTS == 1
            logging::printMsg();
            #endif
            return;
        }

        /**
         * export moves as pgn
         */
        if (this->cmd.compare("_pgn") == 0) {
            std::string expName = gh->getEngineOpt().pgnExportName;
            std::cout << "export game " << expName << std::endl;
            gh->exportPgn(expName.c_str());
            return;
        }

        return;
    }
};


#endif