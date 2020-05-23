#ifndef TREEOPT_H_INCLUDED
#define TREEOPT_H_INCLUDED

/**
 * 
 * Provides Information about tree information
 * such like generation, cuts, termination..
 * 
 */

#include "en_EvalModus.h"
#include "en_NormalizeModus.h"
#include <unordered_map>

struct TreeOpt {

    /**
     * Suchtiefe für normale Züge
     */
    int maxcalcLvl;

    /**
     * Suchtiefe für Schalgzüge
     */
    int maxcalcLvlHit;

    /**
     * Enables the Hash32 to cache evals
     */
    bool useHash32;

    /**
     * Maximale Berechnungszeit wo der Calcloop noch fortgesetzt wird in Sekunden
     */
    int maxCalcTime;

    /**
     * Wie wird der Baum am ende des Hirizints normalisiert
     */
    NormalizeModus normalizeModus;

    /**
     * Gibt an in welchem Level die Berechnung abgebrochen werden soll,
     * wenn für iD gleiche bestMoves erzeugt werden
     */
    int preTermLvl;

    /**
     * max Level where allowed moves checked
     */
    int checkAllowedMoveLevel;

    /**
     * use iterate Deepening
     */
    bool useIterateDeepening;

};

#endif