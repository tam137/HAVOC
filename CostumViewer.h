#ifndef COSTUMVIEWER_H_INCLUDED
#define COSTUMVIEWER_H_INCLUDED

/**
 * Prints game status to std, for debug
 * 
 */

#include "CalcMoveResult.h"
#include "Helper.h"
#include <iostream>

class ConcClass {
    public:
    template <typename T>
    ConcClass conc(T val) {
        std::cout << val;
        return *this;
    }

    ConcClass ln() {
        std::cout << std::endl;
        return *this;
    }
};



class CostumViewer {

private:
    CalcMoveResult result;
    bool uciMode = false;

    CostumViewer() { }

public:
    CostumViewer(bool uciMode) {
        if (!uciMode) {
            //std::cout << "no uci mode" << std::endl;
            this->uciMode = false;
        } else {
            this->uciMode = true;
        }
    }

    template <typename T>
    static ConcClass print(T str) {
        std::cout << str;
        return ConcClass();
    }

    void setCalcResult(const CalcMoveResult &result) {
        this->result = result;
    }

    void printBestMove() {
        if (!(this->uciMode)) {
            Move move = this->result.getBestMove();
            printf("%s-%s (%d)", helper::toNot(move.getFrom()), helper::toNot(move.getTo()), move.getEval());
        }
    }

    void printBestMoveRow() {
        if (!(this->uciMode)) {
            std::vector<Move> bestMoveRow = this->result.getBestMoveRow(); 
            for (int i = 0; i < bestMoveRow.size(); i++) {
                printf(bestMoveRow[i].isHit() ? "%*sx%s" : "%*s-%s", 4, helper::toNot(bestMoveRow[i].getFrom()), helper::toNot(bestMoveRow[i].getTo()));
            }
            printf("%*d", 6, bestMoveRow[0].getEval());
        }
    }

    void printCalcTime() {
        if (!(this->uciMode)) {
            printf(" time: %*d |", 5, this->result.getCalcTime());
        }
    }

    void printNodesPerMs() {
        if (!(this->uciMode)) {
            printf(" perMs: %*d |", 4, this->result.getNodePerMs());
        }
    }

    void printCreatesNodesCount() {
        if (!(this->uciMode)) {
            printf(" NodeSum:%*d |", 8, this->result.getCreatetNodes());
        }
    }

    void printMoveNr() {
        if (!(this->uciMode)) {
            printf(" %*d", 2, this->result.getMoveNr());
        }
    }

    void lineending() {
        if (!(this->uciMode)) {
            std::cout << std::endl;
        }
    }

};

#endif
