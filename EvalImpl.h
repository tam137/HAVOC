#ifndef EVALIMPL_H_INCLUDED
#define EVALIMPL_H_INCLUDED

#include "Helper.h"
#include "en_EvalModus.h"
#include "co_EvalOpt.h"
#include "MoveGen.h"
#include "Board.h"
#include "Move.h"

class Eval {

private:

	int centerPawn = 0;
	int pawnSaves = 0;
	int pawnOff = 0;
	int pawnDouble = 0;
	int moveFredoom = 0;
	int threat = 0;
	int undevelopedFig = 0;
	int kingCastledShort = 0;
	int kingCastledLong = 0;
	int rimKnight = 0;
	int queenStartPosVal = 0;

	int figValues (int*, int);
	int centerPawnValue (int);
	int pawnPressValue (int, bool);
	int undevelopedFigValue (int, bool);
	int kingCastled (int, bool, int);
	int rimKnightValue (int);
	int threatFreedomCalc(int*);
	int queenOnStartPos(int, bool, int);

public:

	// Only in unittests
	char* collectFig (Board *board);
	int getProgValue (char* posList, EvalOpt evalOpt);

	const int evalMove(Board *board, EvalModus evalModus, Move* source, EvalOpt evalOpt);

};

#endif // EVALIMPL