#ifndef MAIN
#define MAIN

#define USEPOSTGRES 0
#define USEASSERTS  0
#define USETHREADS  1
#define USEVARIANCE  0


#include <cassert>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <vector>
#include <list>
#include <unordered_map>
#include <ctime>
#include <unistd.h>


#include "Logger.h"
#include "en_BoardStatus.h"
#include "MoveGen.h"
#include "Helper.h"
#include "en_BoardStatus.h"
#include "Board.h"
#include "Move.h"
#include "Tree.h"
#include "EvalImpl.h"
#include "Gamehandler.h"
#include "co_TreeOpt.h"
#include "CostumViewer.h"
#include "PgnHeader.h"
#include "co_Player.h"
#include "Input.h"
#include "DebugCmd.h"
#include "co_EngineOpt.h"
#include "Unittest.h"
#include "Hash.h"

#if USETHREADS == 1
#include "UciApi.h"
#endif

#if USEPOSTGRES == 1
#include "Postgres.h"
#endif


int main() {

    #if USETHREADS == 1
    UciApi uciApi = UciApi();
    uciApi.uciApiInput();
    #endif

    // settings Tree
    TreeOpt treeOpt;
    treeOpt.maxCalcTime = 20000;
    treeOpt.maxcalcLvl = 4;
    treeOpt.maxcalcLvlHit = 12;
    treeOpt.preTermLvl = treeOpt.maxcalcLvl;
    treeOpt.useHash32 = true;
    treeOpt.normalizeModus = NormalizeModus::allHitNodes;
    treeOpt.checkAllowedMoveLevel = 1;
    treeOpt.useIterateDeepening = true;
    #if USEVARIANCE == 0
    bool bVariance = false;
    #else
    bool bVariance = true;
    #endif

    // settings Eval
    EvalOpt evalOptWhite;
    EvalOpt evalOptBlack;
    EvalOpt evalOptUnittest;
    evalOptWhite.threatFreedomCalc = true;
    evalOptWhite.evalModusHitNodes = EvalModus::deep;
    evalOptWhite.evalModusFullNodes = EvalModus::deep;
    evalOptWhite.centerPawn = 18;
    evalOptWhite.pawnSaves = 4;
    evalOptWhite.pawnOff = 2;
    evalOptWhite.pawnDouble = 0; // unused
    evalOptWhite.undevelopedFig = -30;
    evalOptWhite.kingCastledShort = 45;
    evalOptWhite.kingCastledLong = 20;
    evalOptWhite.rimKnight = -15;
    evalOptWhite.queenStartPosVal = 22;
    evalOptWhite.moveFredoom = 1;
    evalOptWhite.threat = 1;;
    evalOptWhite.pawn = 100;
    evalOptWhite.rook = 500;
    evalOptWhite.knight = 300;
    evalOptWhite.bishop = 300;
    evalOptWhite.queen = 900;
    evalOptWhite.king = 10000;
    evalOptWhite.sumFigValue = helper::getEvalValueOfBoard(evalOptWhite);
    evalOptBlack = evalOptWhite;
    evalOptUnittest = evalOptWhite;

    // settings Player
    Player whitePlayer;
    Player blackPlayer;
    whitePlayer.isHuman = false;
    whitePlayer.name = "HAVOC";
    whitePlayer.evalOpt = evalOptWhite;
    blackPlayer.isHuman = false;
    blackPlayer.name = "Player";
    blackPlayer.evalOpt = evalOptBlack;

    // settings pgn header TODO remove?
    PgnHeader pgnHeader;
    pgnHeader.White = whitePlayer.name;
    pgnHeader.Black = blackPlayer.name;

    // settings Engine
    EngineOpt engineOpt;
    engineOpt.complexityboarder = 0;
    engineOpt.whitePlayer = whitePlayer;
    engineOpt.blackPlayer = blackPlayer;
    engineOpt.treeOpt = treeOpt;
    engineOpt.pgnHeader;
    engineOpt.moveByMove = false;
    engineOpt.pgnExportName = "HavocGame01.pgn";
    engineOpt.useVariance = bVariance;

    // init if uci mode
    #if USETHREADS == 1
    if (uciApi.token.compare("uci") == 0) {
        engineOpt.uciMode = true;
        std::cout << "id name HAVOC" << std::endl;
        std::cout << "id name Jan Lange" << std::endl;
        uciApi.uciGame(engineOpt);
    } else {
        engineOpt.uciMode = false;
    }
    #else
    engineOpt.uciMode = false;
    #endif

    // unittests
    #if USEASSERTS == 1
    if (!(engineOpt.uciMode)) {
        CostumViewer::print("unittests..").ln();
    }
    Unittest *unittest = new Unittest();
    unittest->checkFen();
    unittest->testQuiescence();
    unittest->checkRawMoveList();
    unittest->getHash32();
    unittest->splitMoveStr();
    unittest->queenOnbOard();
    unittest->testGameProgress();
    unittest->BoardFreq();
    unittest->runEvalImpl();
    unittest->checkAllowedMove();
    unittest->castle();
    

    //unittest->testDoublePawnEval(evalOptUnittest);
    #endif


    // engine start prints
    if (!engineOpt.uciMode) {
        CostumViewer::print("I am HAVOC").ln();
        if (whitePlayer.isHuman == true || blackPlayer.isHuman == true) {
            engineOpt.moveByMove = true;
            if (whitePlayer.isHuman) {
                CostumViewer::print("White is Human").ln();
            }
            if (blackPlayer.isHuman) {
                CostumViewer::print("Black is Human").ln();
            } 
        }
        else if (whitePlayer.isHuman == false && blackPlayer.isHuman == false) {
            CostumViewer::print("CPU Game").ln();
        }
        if (engineOpt.useVariance) {
            CostumViewer::print("Use Variance").ln();
        }
    }

    #if USEPOSTGRES == 1
    std::cout << "Use postgres database" << std::endl;
    #endif


    /************************************
     * **********************************
     * The main game loop
     * **********************************
     * **********************************
     ***********************************/

    Gamehandler *gh = new Gamehandler(engineOpt);
    BoardStatus status;
    CalcMoveResult calcResult;
    CostumViewer mv = CostumViewer(engineOpt.uciMode);

    for (int ply = 0; ply < 200; ply++) {
        Player currentPlayer = (ply%2) == 0 ? whitePlayer : blackPlayer;
        std::string input;

        if (engineOpt.moveByMove) {
            if (currentPlayer.isHuman) {
                if (!(engineOpt.uciMode)) { // human user
                    CostumViewer::print("Your move > ");
                }
                std::cin >> input;
                Logger::instance().write(LogLvl::info, input);
            }            

            // check if comand input
            if (input[0] == '_') { 
                DebugCmd cmd = DebugCmd(input, gh, calcResult);
                cmd.runCmd();
                continue;
            }
        }

        // human user
        if (currentPlayer.isHuman) {
            calcResult = input::moveUci(gh, gh->getBoard(), input);
            gh->doMove(calcResult);
            continue;
        } else {

        }

        //stats::reset();        
        calcResult = gh->calcMove();        
        gh->doMove(calcResult);

        mv.setCalcResult(calcResult);
        mv.printMoveNr();
        mv.printCalcTime();
        mv.printNodesPerMs();
        mv.printCreatesNodesCount();
        mv.printBestMoveRow();
        mv.lineending();
        #if USEASSERTS == 1
        #endif
        if (engineOpt.moveByMove) {
            getchar();
        }
    }

    return 0;
}

#endif // MAIN

// #pragma warning(pop)
