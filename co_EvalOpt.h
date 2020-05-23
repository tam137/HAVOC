#ifndef EVALOPT_H_INCLUDED
#define EVALOPT_H_INCLUDED

#include "en_EvalModus.h"


struct EvalOpt {

    /**
     * Evaluationsmodi
     */ 
    EvalModus evalModusHitNodes;

    EvalModus evalModusFullNodes;

	// Figurenwerte
	int pawn;
	int rook;
	int bishop;
	int knight;
	int queen;
	int king;

	// Summe der Figuren-Werte
	int sumFigValue;

	// Punkte für Bauern, die die zentralen Felder besetzen
	int centerPawn;

	// Punkte für Bauern die eine eigene Figur decken
	int pawnSaves;

	// Punkte für Bauern die vorgerückt sind (pro Reihe)
	int pawnOff;

	// malus pawn double
	int pawnDouble;

	// Punkte für Bewegungsfreiheit pro Feld
	int moveFredoom;

	// Punktefaktor für Drohungen gegen den Gegner
	int threat;

	// Malus für Springer und Läufer auf der Grundreihe
	int undevelopedFig;

	// könig steht auf g1 / g8 oder c1 / c8 (Rochade)
	int kingCastledShort;
	int kingCastledLong;

	// Malus für Springer am Rand
	int rimKnight;

	// Berechnet Drohungs und Freiheitswerte
	bool threatFreedomCalc;

	// Bonus für Dame in Startpos
	int queenStartPosVal;

};


#endif // EVALOPT_H_INCLUDED