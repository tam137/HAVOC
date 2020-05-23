#ifndef EVALMODUS_H_INCLUDED
#define EVALMODUS_H_INCLUDED

/**
 * EvalModus gestzt von Eval.h verwendet in EvalImpl.h und Tree.h
 */


enum EvalModus {
    
    // vollständige Berechnung - alle Bewertungensmöglichkeiten
    deep = 1,

    // Berechnet nur den Wert der Figuren - figOnloy()
    onlyFig = 2,

    // eval aus aus HashMap mapHash32
    mapHash32 = 3,

    // Wenn noch nicht bewertet (Initalisierung) - Bewertung 0 initialisiert
    none = 9
};

#endif // EVALMODUS