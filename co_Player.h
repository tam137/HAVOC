#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string.h>
#include "co_EvalOpt.h"

struct Player {

    std::string name;

    bool isHuman;

    EvalOpt evalOpt;

};

#endif