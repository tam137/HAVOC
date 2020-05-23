#ifndef HELPER_CPP_INCLUDED
#define HELPER_CPP_INCLUDED

#include "Helper.h"

#define USEASSERTS  0

// TODO use memcpy
void helper::init_brett(int brett[], const int brett_vgl[]) {
    for (int i = 0; i<120; i++){
        brett[i] = brett_vgl[i];
    }
    return;
}

bool helper::sortEvalAsc(Move m1, Move m2) {
    return m1.getEval() < m2.getEval();
}

bool helper::sortEvalDes(Move m1, Move m2) {
    return m1.getEval() > m2.getEval();
}

bool helper::sortHitDes(Move m1, Move m2) {
    return m1.isHit() > m2.isHit();
}

bool helper::sortPrioDes(Move m1, Move m2) {
    return m1.getPrio() > m2.getPrio();
}

bool helper::sortPrioAsc(Move m1, Move m2) {
    return m1.getPrio() < m2.getPrio();
}

/**
 * Map Fields to notification
 */
const char* helper::toNot(int field) {
    return FieldMapping::getFieldNotation(field);
}

/**
 * returns a random integer 0 till exclude max
 */
int helper::getRandom(int max) {
    //srand (time(NULL));
    srand (std::clock());
    return rand() % max;
}

/**
 * Returns a String vor a given Move Row Vector 
 * 
 */
const char* helper::getMoveRowString(std::vector<Move> moveRowVector) {
    char moveRow[1023];
    strcpy(moveRow, "");
    int i = 0;
    do {
        strcat(moveRow, toNot(moveRowVector[i].getFrom()));
        strcat(moveRow, moveRowVector[i].isHit() ? "x" : "-");
        strcat(moveRow, toNot(moveRowVector[i].getTo()));
        i++;
        if (i < moveRowVector.size()) strcat(moveRow, " ");
        else break;
        
    } while (true);
    return moveRow;
}

/**
 * Converts a Move object to Notification string
 */
const char* helper::getNotStr(Move move) {
    char notStr[63];
    strcpy(notStr, toNot(move.getFrom()));
    strcat(notStr, move.isHit() ? "x" : "-");
    strcat(notStr, toNot(move.getTo()));
    return notStr;
}

/**
 * Eval-Value of all figs at beginning of the game
 * used to init sumFigValues for evalOpt
 */
int helper::getEvalValueOfBoard(EvalOpt evalOpt) {
    int value = 0;
    value += evalOpt.pawn * 8;
    value += evalOpt.knight * 2;
    value += evalOpt.bishop * 2;
    value += evalOpt.rook * 2;
    value += evalOpt.queen * 1;
    value *= 2;
    return value;
}

/**
 * Return max of two ints
 */
int helper::max(int x, int y) {
    return x > y ? x : y;
}

/**
 * Return min of to ints
 */
int helper::min(int x, int y) {
    return x < y ? x : y;
}

int helper::absolut(int x, int y) {
    if (x > y) return x - y;
    else return y - x;
}

/**
 * Gets a Fig as int
 * returns the Value of the Fig in centipawns
 */
int helper::mapFigToValue(int figure, EvalOpt evalOpt) {
    if (figure == 0) { return evalOpt.pawn; }
    else if (figure == 1) { return evalOpt.rook; }
    else if (figure == 2) { return evalOpt.knight; }
    else if (figure == 3) { return evalOpt.bishop; }
    else if (figure == 4) { return evalOpt.queen; }
    else if (figure == 5) { return evalOpt.king; }
    #if USEASSERTS == 1
    else { assert (false); }
    #endif
    return 0;
}

/**
 * Returns the Array size of a char array
 * arr the array
 * allocSize the alloc size
 * return index of first occurence of '\0'
 */
int helper::arraySize(char* arr, int allocSize) {
    #if USEASSERTS == 1
    assert (arr[allocSize - 1] == '\0');
    #endif
    for (int i = 0; i < allocSize; i++) {
        if (arr[i] == '\0') {
            return i;
        }
    }
    #if USEASSERTS == 1
    assert (false);
    #endif
    return -1;
}

#endif