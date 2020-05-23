#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

#include <list>
#include <vector>

#include "Helper.h"

class Movegen {

    private:


    public:
    Movegen();

    std::list<Move> *getMoveList(int* brett, Move* preMove, bool white, bool hitMoves = false);
    std::list<Move> *getMovesHit(int brett[], Move *preMove, bool white, bool castle = false);
    std::list<Move> *getMovesQuite(int brett[], Move *preMove, bool white);
    char* getRawList(int brett[], bool hitMoves, bool white);

};

#endif