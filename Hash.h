#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <unordered_map>

#include "Board.h"


class Hash {
    
    private:
        // hash32 - eval in Tree
        std::unordered_map<unsigned int, int> mapHash32;

        // hash32 - frequency in GH
        std::unordered_map<unsigned int, char> boardFreq;

    public:
        unsigned int get32BitHash(int *brett);
        int getEvalForHash32(unsigned int);
        bool hasHash32(unsigned int hash32);
        int hash32Size();
        void resetHash32();
        void add32Hash();
        void addBoardFreq(Board &board);
        int getBoardFreq(unsigned int hash32);
        void reset();
        
    };


#endif