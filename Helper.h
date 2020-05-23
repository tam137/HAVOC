#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <ctime>
#include <cstring>

#include "FieldMapping.h"
#include "co_EvalOpt.h"
#include "Move.h"


namespace helper {

    void init_brett(int brett[], const int brett_vgl[]);
    bool sortEvalAsc(Move m1, Move m2);
    bool sortEvalDes(Move m1, Move m2);
    bool sortHitDes(Move m1, Move m2);
    bool sortPrioDes(Move m1, Move m2);
    bool sortPrioAsc(Move m1, Move m2);
    int getEvalValueOfBoard(EvalOpt evalOpt);
    int max(int x, int y);
    int min(int x, int y);
    int absolut(int x, int y);
    int mapFigToValue(int figure, EvalOpt evalOpt);
    int getRandom(int max);
    const char* toNot(int field);    
    const char* getMoveRowString(std::vector<Move> moveRowVector);
    const char* getNotStr(Move move);
    int arraySize(char*, int allocSize);
}

#endif