#ifndef CASTLEMODUS_H_INCLUDED
#define CASTLEMODUS_H_INCLUDED

/**
 * CastleModus
 */
enum CastleModus {

    // Kann nicht mehr rochieren
    noneC = 0,

    // kann in beide richtungen rochieren
    both = 1,

    // kann nur lang rochieren
    longCastle = 2,

    // kann nur kurz rochieren
    shortCastle = 3,

};

#endif // CASTLEMODUS