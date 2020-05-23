#ifndef UCIAPI_CPP_INCLUDED
#define UCIAPI_CPP_INCLUDED

#include "UciApi.h"

void UciApi::sendUciOk() {
    std::cout << "uciok" << std::endl;
    Logger::instance().write(LogLvl::info, "UCIOUT: uciok");
}
void UciApi::sendReadyOk() {
    std::cout << "readyok" << std::endl;
    Logger::instance().write(LogLvl::info, "UCIOUT: readyok");
}

void UciApi::sendBestMove(CalcMoveResult moveResult) {
    std::string uciString = moveResult.getBestMove().toUciString();
    uciString = "bestmove " + uciString;
    Logger::instance().write(LogLvl::info, "UCIOUT: " + uciString);
    std::cout << uciString << std::endl;
    return;
}

void UciApi::sendCalcInfo(CalcMoveResult moveResult) {

    return;
}

void UciApi::uciApiInput() {
    char bufInput[4095];
    std::cin.getline(bufInput, 4095);
    token = bufInput;
    Logger::instance().write(LogLvl::info, "UCIIN: " + token);
    return;
}

void UciApi::uciGame(EngineOpt engineOpt) {

    UciApi::sendUciOk();

    Gamehandler *gh = new Gamehandler(engineOpt);
    BoardStatus status;
    CalcMoveResult calcResult;

    while (true) {
        std::thread thread(&UciApi::uciApiInput, this);
        thread.join();

        // ISREADY
        if (UciApi::token.rfind("isready", 0) == 0) {
            UciApi::sendReadyOk();
        }
        
        // UCINEWGAME
        else if (UciApi::token.rfind("ucinewgame", 0) == 0) {
            gh = new Gamehandler(engineOpt);
        }
        
        // GO
        else if (UciApi::token.rfind("go", 0) == 0) {
            CalcMoveResult result = gh->calcMove();
            gh->doMove(result);
            UciApi::sendCalcInfo(result);
            UciApi::sendBestMove(result);
        }

        // MOVES
        else if (UciApi::token.find("moves") != std::string::npos) {
            int idx = UciApi::token.find("moves");
            std::string moveListStr = UciApi::token.substr(idx+6);
            std::vector<std::string> moveV = moveHelper::splitUciMoveRow(moveListStr);
            std::vector<std::string> moveLast;
            moveLast.push_back(moveV.at(moveV.size()-1));
            gh->doMoveList(moveLast, true);

        // UNKNOWN
        } else {
            //Logger::instance().write(LogLvl::info, "unknown token ");
        }
    }
    gh = NULL;
    delete gh;
}


#endif