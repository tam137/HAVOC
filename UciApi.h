#ifndef UCIAPI_H_INCLUDED
#define UCIAPI_H_INCLUDED

#include <iostream>
#include <thread>

#include "Logger.h"
#include "CalcMoveResult.h"
#include "Gamehandler.h"

class UciApi {
    public:
    std::string token;
    void sendUciOk();
    void sendReadyOk();
    void sendBestMove(CalcMoveResult moveResult);
    void sendCalcInfo(CalcMoveResult moveResult);
    void uciApiInput();
    void uciGame(EngineOpt engineOpt);
};

#endif