#ifndef ENGINEOPT_H_INCLUDED
#define ENGINEOPT_H_INCLUDED

#include "co_Player.h"
#include "co_TreeOpt.h"
#include "co_EvalOpt.h"
#include "PgnHeader.h"


struct EngineOpt {

    Player whitePlayer;

    Player blackPlayer;

    TreeOpt treeOpt;

    PgnHeader pgnHeader;

    std::string pgnExportName;

    /**
     * Move by Move, enable userinput after each move
     */
    bool moveByMove;

    /**
     * Engine is in uci mode
     */
    bool uciMode;

    /**
     * Uses variance, dont use every time the best Move
     */
    bool useVariance;

    /**
     * calc depper when board com plexity less then complexityboarder
     */
    int complexityboarder;

};


#endif // ENGINEOPT_H_INCLUDED