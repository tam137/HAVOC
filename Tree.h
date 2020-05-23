#ifndef TREE_CPP_INCLUDED
#define TREE_CPP_INCLUDED


#include "Hash.h"
#include "co_TreeOpt.h"
#include "co_EngineOpt.h"
#include "en_TreeModus.h"
#include "en_NormalizeModus.h"
#include "algorithm"
#include "EvalImpl.h"
#include "Move.h"
#include "Board.h"


class Tree {
private:
    std::list<Move> nodeList;
    std::vector<Move*> firstLevelNodeList;
    std::unordered_map<long, Move*> mapMoveNr;
    std::vector<std::list<Move>*> ptrs;
    std::unordered_map<int, char> prioMoveList;
    Board board;
    Hash* hash;
    TreeOpt treeOpt;
    EngineOpt engineOpt;
    int hash32uses;
    int allFigValue;
    int initEval;
    bool white;
    int minMaxCreatedNodes;
    int sumCreatedNodes;

public:

    Tree();
    ~Tree();
    Tree(Board board, bool white, EngineOpt engineOpt, Hash* hash);
    Tree(Board board, bool white, EngineOpt engineOpt, std::unordered_map<int, char> prioMoveList, int initEval, Hash* hash);
    int alphaBetaExp(Board &board, int level, int maxLvl, int localHitLvl, Move *preMove, int alpha, int beta, bool white);
    std::vector<Move> getMoveRow(const Move &move);
    std::vector<Move> getSortedMovesFirstLvl();
    Move getBestMove();
    std::list<Move*> getMovesFirstLvlPtr();
    std::vector<long> getMovesFirstLvlMoveId();
    bool hasNodeForMoveId(int moveId);
    TreeOpt getTreeOpt();
    bool hasHash32(unsigned int hash32);
    int getEvalForHash32(unsigned int hash32);
    std::list<Move> getNodeList();
    int getMoveCount() const;
    const Board getBoard();
    int getEvalFromBestMove();
    int getHash32UsesCount();
    int getMinMaxCreatedNodes();
    void resetMinMaxCreatedNodes();
    int getSumCreatedNodes();
    int quiescence(Board &board, int level, Move *preMOve, int alpha, int beta, bool white);

};

#endif