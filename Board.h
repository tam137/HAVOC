#ifndef BOARD_CPP_INCLUDED
#define BOARD_CPP_INCLUDED


#include <cstring>
#include <iostream>

#include "en_CastleModus.h"
#include "en_BoardStatus.h"
#include "co_EvalOpt.h"
#include "MoveGen.h"
#include "Helper.h"

class Board {
private:
    
    int brett[120] = {
		
		-11, -11, -11, -11, -11, -11, -11, -11, -11, -11,
		-11, -11, -11, -11, -11, -11, -11, -11, -11, -11,		
		//   a   b   c   d   e   f   g   h			
		-11, 21, 22, 23, 24, 25, 23, 22, 21, -11,//20 - 8
		-11, 20, 20, 20, 20, 20, 20, 20, 20, -11,//30 - 7
		-11,  0,  0,  0,  0,  0,  0,  0,  0, -11,//40 - 6
		-11,  0,  0,  0,  0,  0,  0,  0,  0, -11,//50 - 5
		-11,  0,  0,  0,  0,  0,  0,  0,  0, -11,//60 - 4
		-11,  0,  0,  0,  0,  0,  0,  0,  0, -11,//70 - 3
		-11, 10, 10, 10, 10, 10, 10, 10, 10, -11,//80 - 2
		-11, 11, 12, 13, 14, 15, 13, 12, 11, -11,//90 - 1
		//    1   2   3   4   5   6   7   8 <- Indexbezeichnungen
		-11, -11, -11, -11, -11, -11, -11, -11, -11, -11,
		-11, -11, -11, -11, -11, -11, -11, -11, -11, -11
	};

    EvalOpt evalOptW;
    EvalOpt evalOptB;

    CastleModus whiteCastle;
    CastleModus blackCastle;

public:

    Board();
    Board(EvalOpt evalOptW, EvalOpt evalOptB);
    Board(EvalOpt evalOptW, EvalOpt evalOptB, int* brett, CastleModus whiteC, CastleModus blackC);
    Board* initBoardForFen(std::string fen);
    void clearBoard();
    int* getBoard();
    #if USEASSERTS == 1
    void setField(int index, int value);
    #endif
    bool indexHasFigure(int index);
    int getFigureAtIndex(int index);
    int getFigValueAtIndex(int index, bool white);
    bool getWhiteCastleModus();
    bool getBlackCastleModus();
    void printBoard();
    void performMoves(std::vector<std::string> moveList, bool white);
    void execSingleMoveOnBoard(Move* move);
    Board *cpyBoard();
    BoardStatus getBoardStatus();
    Board* returnBoardForMoves(Move* move);
    bool isAllowedMove (Move move);
    bool queenOnBoard();
    int getComplexity();

};

#endif