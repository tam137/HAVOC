#ifndef GAMEHANDLER_H_INCLUDED
#define GAMEHANDLER_H_INCLUDED

#include "Tree.h"
#include "Hash.h"
#include "CalcMoveResult.h"
#include "PgnExport.h"

//#include "UciApi.h"



class Gamehandler {
private:
    Board board;
    std::vector<CalcMoveResult> moveResults;
    Tree *lastTree = new Tree();
    std::unordered_map<int, char> prioMoveList;
    EngineOpt engineOpt;
    PgnHeader pgnHeader;
    TreeOpt treeOpt;
    long int startTimeInt;
    Hash* hash = new Hash();

    Gamehandler ();
    int getLastEval();
    long int getMoveCalcTime();

public:
    
    Gamehandler (EngineOpt engineOpt);
    ~Gamehandler ();

    void doMove(const CalcMoveResult &result);
    void doMoveList(std::vector<std::string> moveList, bool white);
    CalcMoveResult calcMove();
    int calcTimeMs();
    std::vector<Move> getBestMoveRow(Move move) const;
    void exportPgn(char const filename[]);
    Board getBoard();
    std::vector<CalcMoveResult> getMoveResults();
    EngineOpt getEngineOpt();
    int getMoveCount();
    Tree *getLastTree();
    void addMoveToPrioList(Move move, int prio);

};

#endif
