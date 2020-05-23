#ifndef HASH_CPP_INCLUDED
#define HASH_CPP_INCLUDED

#include "Hash.h"

#define USEASSERTS  0

/**
 * Hashes beginnen die Berechnung bei Feld 20 bis 99
 * Die anderen Felder 0 bis 19 und 100 bis 119 haben keinen einfluss auf den Hash
 * 
 */


unsigned int Hash::get32BitHash(int *brett) { // TODO Board stat brett
    unsigned int hash32 = 990990;
    
    for (int i = 20; i < 100; i++) {
        if (hash32 < 10) { hash32+=990; hash32*=99; }
        if (brett[i] == 0)  { hash32+=11; hash32*=137; continue; }
        if (brett[i] == -11)  { hash32+=17; hash32*=37; continue; }
        else { hash32+=37+i; hash32*=brett[i]; continue; }
    }

    for (int i = 20; i < 100; i++) {
        if (hash32 < 10) { hash32+=990; hash32*=99; }
        if (brett[i] == 0)  { hash32+=11; hash32*=137; continue; }
        if (brett[i] == -11)  { hash32+=17; hash32*=37; continue; }
        else { hash32+=127+i; hash32+=brett[i]; continue; }
    }

    return hash32;
}

/**
 * Adds the boardsituation to hashmap, count++
 */
void Hash::addBoardFreq(Board &board) {
    unsigned int hash32 = this->get32BitHash(board.getBoard());
    if (this->boardFreq.count(hash32)) {
        this->boardFreq[hash32] = ++this->boardFreq[hash32];
    } else {
        this->boardFreq[hash32] = 1;
    }
    return;
}

/**
 * Gets the previosly count of boardsituations
 */
int Hash::getBoardFreq(unsigned int hash32) {
    if (boardFreq.count(hash32)) {
        return boardFreq[hash32];
    } else {
        return 0;
    }
}

int Hash::hash32Size() {
    return this->mapHash32.size();
}

int Hash::getEvalForHash32(unsigned int hash32) {
    if (this->mapHash32.count(hash32)) {
        #if USEASSERTS == 1
        assert (this->mapHash32.count(hash32) == 1);
        #endif
        return mapHash32[hash32];
    } else {
        #if USEASSERTS == 1
        assert (false);
        #endif
        return 0;
    }    
}

bool Hash::hasHash32(unsigned int hash32) {
    if (this->mapHash32.count(hash32)) {
        #if USEASSERTS == 1
        assert (this->mapHash32.count(hash32) == 1);
        #endif
        return true;
    } else {
        return false;
    }        
}

void Hash::resetHash32() {
    this->mapHash32.clear();
    return;
}

void Hash::reset() {
    this->mapHash32.clear();
    this->boardFreq.clear();
    return;
}



#endif