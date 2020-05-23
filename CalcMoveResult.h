#ifndef CALCMOVERESULT_H_INCLUDED
#define CALCMOVERESULT_H_INCLUDED

#include <map>

#include "co_EngineOpt.h"
#include "en_BoardStatus.h"
#include "Move.h"
#include "Tree.h"

/**
 * Provides all Information to the Gamehandler about
 * the recently move calculation and gamestatus
 */


class CalcMoveResult {

private:
    BoardStatus status;
    Move bestMove;
    std::vector<Move> bestMoveRow;    
    int calcTime;
    int createtNodes;
    int moveNr;
    Tree* lastTree;
    EngineOpt engineOpt;
    

public:

    CalcMoveResult() { };

    void setBestMove(Move move) {
        this->bestMove = move;
    }

    void setBestMoveRow(std::vector<Move> bestMoveRow) {
        this->bestMoveRow = bestMoveRow;
    }

    void setCalcTime(long calcTime) {
        this->calcTime = calcTime;
    }

    void setCreatedNodesCount(int nodeCount) {
        this->createtNodes = nodeCount;
    }

    void setBoardStatus(BoardStatus status) {
        this->status = status;
    }

    void setMoveNr(int moveNr) {
        this->moveNr = moveNr;
    }

    void setLastTree(Tree* tree) {
        this->lastTree = tree;
    }

    Tree* getLastTree() {
        return this->lastTree;
    }    

    Move getBestMove() const { return this->bestMove; }
    std::vector<Move> getBestMoveRow() const { return this->bestMoveRow; }
    int getCalcTime() const { return (this->calcTime / 1000) + 1; }
    int getCreatetNodes() const { return this->createtNodes; }
    int getNodePerMs() const { return this->createtNodes / this->getCalcTime(); }
    BoardStatus getBoardStatus() const { return this->status; }
    int getMoveNr() const { return this->moveNr; }
};

#endif