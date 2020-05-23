#ifndef TREEMODUS_H_INCLUDED
#define TREEMODUS_H_INCLUDED

/**
 * TreeModus
 * Art und Weise auf die ein Knoten erzeugt wurde
 * fulll expansed node
 * hitNodes only expanse hit-Moves
 * quiteNodes only expanse non-hit-Moves (experimental)
 */
enum TreeModus {

    // fulll expansed node
    full = 1,
    
    // hitNodes only expanse hit-Moves
    hitNode = 2,

    // quiteNodes only expanse non-hit-Moves (experimental)
    quiteNode = 3,

    unset = 9
};

#endif // TREEMODUS