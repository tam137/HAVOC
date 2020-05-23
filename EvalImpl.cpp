#ifndef EVALIMPL_CPP_INCLUDED
#define EVALIMPL_CPP_INCLUDED

#include "EvalImpl.h"

#define USEASSERTS  0


		//////////////////////////////////////////////////////////////
		//															//
		//					Evaluierungsfunktionen   				//
		//					White									//
		//															//
		//////////////////////////////////////////////////////////////



/**
 * Evaluiert die Figurenwerte
 */
int Eval::figValues(int* brett, int i) {
	int evalBuf = 0;
	if (brett[i] == 10){
		return 100;
	}
	else if (brett[i] == 11){
		return 500;
	}
	else if (brett[i] == 12){
		return 300;
	}
	else if (brett[i] == 13){
		return 300;
	}
	else if (brett[i] == 14){
		return 900;
	}
	else if (brett[i] == 15){
		return 10000;
	}
	else if (brett[i] == 20){
		return -100;
	}
	else if (brett[i] == 21){
		return -500;
	}
	else if (brett[i] == 22){
		return -300;
	}
	else if (brett[i] == 23){
		return -300;
	}
	else if (brett[i] == 24){
		return -900;
	}
	else if (brett[i] == 25){
		return -10000;
	}
	else {
		#if USEASSERTS == 1
		assert (false);
		#endif
	}
	return 0;
}

/**
 * Punkte für Zentrale Bauern Figuren
 */
int Eval::centerPawnValue(int i) {
	char centerFields[4] = { 54, 55, 64, 65 };
	if (i == 54 || i == 55 || i == 64 || i == 65) {
		return centerPawn;
	}
	else return 0;
}

/**
 * Punkte für Bauernvorstöße
 */
int Eval::pawnPressValue(int i, bool white) {
	// Checke ob Bauern Figuren decken
	//if (brett[i+9] / 10 == 1 || brett[i+11] / 10 == 1) {
	//	evalBuf += pawnSaves;
	//}
	// Checke wie weit die Bauern vorgerückt sind
	int evalBuf = 0;
	int pawnRow = 10 - (i / 10);
	if (white) {
		pawnRow = 10 - (i / 10);
	}
	else {
		pawnRow = -1 + (i / 10);
	}
	evalBuf += pawnRow * pawnOff;
	if (pawnRow >= 6) evalBuf += pawnRow * pawnOff*2;
	if (pawnRow >= 7) evalBuf += pawnRow * pawnOff*3;
	if (pawnRow >= 8) evalBuf += pawnRow * pawnOff*4;
	return evalBuf;
}

/**
 * Malus für Figuren (Spring & Läufer) auf Grundreihe
 */
int Eval::undevelopedFigValue(int i, bool white) {
	if (white && i >= 91 && i <= 98) {
		return undevelopedFig;
	}
	if (!white && i >= 21 && i <= 28) {
		return undevelopedFig;
	}
}

/**
 * Punkte für König hat rochiert
 */
int Eval::kingCastled(int i, bool white, int progVal) {
	if (progVal < 120) {
		return kingCastledLong / 2;
	}
	if (white) {
		if (i == 97) { return kingCastledShort; }
		if (i == 93) { return kingCastledLong; }
	} else {
		if (i == 27) { return kingCastledShort; }
		if (i == 23) { return kingCastledLong; }
	}
	return 0;
}

/**
 * Berechnet um Springer am Rand
 */
int Eval::rimKnightValue(int i) {
	if (i%10 == 1 || i%10 == 8) { return rimKnight; } // reihe A und H
	else return 0;
}

/**
 * Berechnet Zugfreiheit und Drohwert
 */

int Eval::threatFreedomCalc(int* brett) {

	int evalBuf = 0;
	Movegen *movegen = new Movegen();

	char rawArrW[256] = { '\0' };
	char rawArrB[256] = { '\0' };
	memcpy(rawArrW, movegen->getRawList(brett, false, true), 256 * sizeof(char));
	memcpy(rawArrB, movegen->getRawList(brett, false, false), 256 * sizeof(char));

	int sizeW = helper::arraySize(rawArrW, 256);
	int sizeB = helper::arraySize(rawArrB, 256);

	evalBuf += sizeW / 2;
	evalBuf -= sizeB / 2;
	
	for (int i = 1; i < sizeW; i = i + 2) {
		if (brett[rawArrW[i]] == 20) evalBuf += threat;
		else if (brett[rawArrW[i]] == 21) evalBuf += threat * 5;
		else if (brett[rawArrW[i]] == 22) evalBuf += threat * 3;
		else if (brett[rawArrW[i]] == 23) evalBuf += threat * 3;
		else if (brett[rawArrW[i]] == 24) evalBuf += threat * 9;
		else if (brett[rawArrW[i]] == 25) evalBuf += threat * 20;
		else if (brett[rawArrW[i]] == 0) { ;} // do nothing
		else assert (false);
	}
	for (int i = 1; i < sizeB; i = i + 2) {
		if (brett[rawArrB[i]] == 10) evalBuf -= threat;
		else if (brett[rawArrB[i]] == 11) evalBuf -= threat * 5;
		else if (brett[rawArrB[i]] == 12) evalBuf -= threat * 3;
		else if (brett[rawArrB[i]] == 13) evalBuf -= threat * 3;
		else if (brett[rawArrB[i]] == 14) evalBuf -= threat * 9;
		else if (brett[rawArrB[i]] == 15) evalBuf -= threat * 20;
		else if (brett[rawArrB[i]] == 0) { ;} // do nothing
		else assert (false);
	}
	return evalBuf;
}

int Eval::queenOnStartPos(int i, bool white, int progVal) {
	if (progVal > 200) {
		if (!white && i == 24 || i / 10 == 3) { return queenStartPosVal; }
		if (white && i == 94 || i / 10 == 8) { return queenStartPosVal; }
		return 0;
	} else {
		return queenStartPosVal;
	}
	
}



		//////////////////////////////////////////////////////////////
		//															//
		//					Hilfsfunktionen			   				//
		//															//
		//															//
		//////////////////////////////////////////////////////////////



/**
 * collect all fig (black + white) and pos
 */
char* Eval::collectFig (Board *board) {
	//std::vector<char> ret;
	static char ret[65] = { '\0' };
	for (int i = 0; i <= 65; i++) {
		ret[i] = '\0';
	}

	int brett[120];
	helper::init_brett(brett, board->getBoard());

	int ptr = -1;
	for (int i = 21; i < 99; i++) {
		if (brett[i] / 10 == 1 || brett[i] / 10 == 2) {
			ret[++ptr] = (brett[i]); // was
			ret[++ptr] = (i);		 // wo
		}
	}
	return ret;
}

/**
 * returns the gameprogres value
 * 255 at begin, 0 at endgame
 *
 */
int Eval::getProgValue (char* posList, EvalOpt evalOpt) {
	int progValue = -1;
	int figValues = 0;
	int posLSize = helper::arraySize(posList, 65);
	for (int i = 0; i < posLSize; i=i+2) {
		if (posList[i]%10 == 0) {
			figValues += evalOpt.pawn;
		}
		if (posList[i]%10 == 1) {
			figValues += evalOpt.rook;
		}
		if (posList[i]%10 == 2) {
			figValues += evalOpt.knight;
		}
		if (posList[i]%10 == 3) {
			figValues += evalOpt.bishop;
		}
		if (posList[i]%10 == 4) {
			figValues += evalOpt.queen;
		}
	}
	progValue = (figValues*255) / evalOpt.sumFigValue;
	return progValue;
}



		//////////////////////////////////////////////////////////////
		//															//
		//				Eval the given board here   				//
		//															//
		//															//
		//////////////////////////////////////////////////////////////



const int Eval::evalMove(Board *board, EvalModus evalModus, Move* source, EvalOpt evalOpt) {

	//stats::evaluatedNodes++;
	
	// init vars
	int brett[120];
	helper::init_brett(brett, board->getBoard());
	int evalBuf = 0;
	int evalValueWhite = 0;
	int evalValueBlack = 0;

	#if USEASSERTS == 1
	if (source != NULL) {
		source->addEvalModus(evalModus);
	}
	#endif

	// calc game progress + position list (0 = was, 1 = wo)
	char* posList = Eval::collectFig(board);
	int progValue = Eval::getProgValue(posList, evalOpt);


	// init eval values
	centerPawn = evalOpt.centerPawn;
	pawnSaves = evalOpt.pawnSaves;
	pawnOff = evalOpt.pawnOff;
	pawnDouble = evalOpt.pawnDouble;
	moveFredoom = evalOpt.moveFredoom;
	threat = evalOpt.threat;
	undevelopedFig = evalOpt.undevelopedFig;
	kingCastledShort = evalOpt.kingCastledShort;
	kingCastledLong = evalOpt.kingCastledLong;
	rimKnight = evalOpt.rimKnight;
	queenStartPosVal = evalOpt.queenStartPosVal;
	
	evalBuf = 0;

	// Drohungswerte berechnen ja / nein // TODO für schwarz
	if (evalOpt.threatFreedomCalc) {
		evalBuf += Eval::threatFreedomCalc(brett);
	}
		

	int k = 1; // zähler über poslist (wo)
	for (int i = 21; i < 99; i++) {
		#if USEASSERTS == 1
		int posListS = helper::arraySize(posList, 65);
		if (k > posListS) {
			assert (brett[98] == 0);
			break;
		}
		#endif

		if (i == posList[k]) { // *i = was, *k = wo
			int where = i;
			int what = brett[i];
			//bool white = (what >= 20) ? false : true;

			evalBuf += Eval::figValues(brett, i);

			if (what == 10) {
				evalBuf += Eval::centerPawnValue(i);
				evalBuf += Eval::pawnPressValue(i, true);
				k = k + 2;					
				continue;
			}
			else if (what == 11) {
				k = k + 2;
				continue;
			}
			else if (what == 12) {
				evalBuf += Eval::undevelopedFigValue(i, true);
				evalBuf += Eval::rimKnightValue(i);
				k = k + 2;
				continue;
			}
			else if (what == 13) {
				evalBuf += Eval::undevelopedFigValue(i, true);
				k = k + 2;
				continue;
			}
			else if (what == 14) {
				evalBuf += Eval::queenOnStartPos(i, true, progValue);
				k = k + 2;
				continue;
			}
			else if (what == 15) {
				evalBuf += Eval::kingCastled(i, true, progValue);
				k = k + 2;
				continue;
			} 
			else if (what == 20) {
				evalBuf -= Eval::centerPawnValue(i);
				evalBuf -= Eval::pawnPressValue(i, false);
				k = k + 2;					
				continue;
			}
			else if (what == 21) {
				k = k + 2;
				continue;
			}
			else if (what == 22) {
				evalBuf -= Eval::undevelopedFigValue(i, false);
				evalBuf -= Eval::rimKnightValue(i);
				k = k + 2;
				continue;
			}
			else if (what == 23) {
				evalBuf -= Eval::undevelopedFigValue(i, false);
				k = k + 2;
				continue;
			}
			else if (what == 24) {
				evalBuf -= Eval::queenOnStartPos(i, false, progValue);
				k = k + 2;
				continue;
			}
			else if (what == 25) {
				evalBuf -= Eval::kingCastled(i, false, progValue);
				k = k + 2;
				continue;
			}
			else {
				#if USEASSERTS == 1
				assert (false);
				#endif
			}
		}
	}

	// count eval sum and return calculated eval
	//evalSum++;
	return evalBuf;
}



#endif // EVALIMPL