#ifndef NORMALIZEMODUS_H_INCLUDED
#define NORMALIZEMODUS_H_INCLUDED

/**
 * normalize the tree
 */
enum NormalizeModus {

    // no normalize
    noNormalize = 0,

    // normalize to even hit nodes
    evenHitNodes = 1,

    // normalisiere hitMoves
    allHitNodes = 2

};

#endif // NORMALIZEMODUS