#ifndef UNITTEST_H_INCLUDED
#define UNITTEST_H_INCLUDED

#include "Tree.h"
#include "Board.h"
#include "Helper.h"
#include "EvalImpl.h"
#include "Hash.h"

#define USEASSERTS  0

#if USEASSERTS == 1

class Unittest {

private:

    EngineOpt engineOpt;

    Move genTestMove(std::string moveStr, bool white) {
        char from[3] = { moveStr[0], moveStr[1] };
        char to[3] = { moveStr[2], moveStr[3] };
        from[2] = '\0';
        to[2] = '\0';
        int fromInt = FieldMapping::getFieldIndex(from);
        int toInt = FieldMapping::getFieldIndex(to);

        Move move = Move(fromInt, toInt, 0, 1, NULL, white);
        return move;
    }

public:

    Unittest () {

        // settings Tree
        TreeOpt treeOpt;
        treeOpt.maxCalcTime = 20000;
        treeOpt.maxcalcLvl = 4;
        treeOpt.maxcalcLvlHit = 20;
        treeOpt.preTermLvl = treeOpt.maxcalcLvl;
        treeOpt.useHash32 = true;
        treeOpt.normalizeModus = NormalizeModus::allHitNodes;
        treeOpt.checkAllowedMoveLevel = 2;
        bool bVariance = false;

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

        this->engineOpt.whitePlayer = whitePlayer;
        this->engineOpt.blackPlayer = blackPlayer;
        this->engineOpt.treeOpt = treeOpt;
        this->engineOpt.pgnHeader;
        this->engineOpt.moveByMove = false;
        this->engineOpt.pgnExportName = "HavocGame01.pgn";
        this->engineOpt.useVariance = bVariance;
    }

    void checkFen() {
        Board* board = new Board();
        board->initBoardForFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        // test startpos

        board->initBoardForFen("rnbqkbnr/pp1ppppp/2p5/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 2");
        assert (board->getFigureAtIndex(43) == 20);
        assert (board->getFigureAtIndex(64) == 10);
        assert (board->getFigureAtIndex(33) == 0);
        assert (board->getFigureAtIndex(84) == 0);

        delete board;
        return;
    }


    void testQuiescence() {
        Board* board = new Board();
        board->clearBoard();

        this->engineOpt.treeOpt.normalizeModus = NormalizeModus::noNormalize;

        board->setField(21, 25);
        board->setField(91, 15);
        board->setField(44, 20); // black pawn d6
        board->setField(55, 22); // black knight e5
        board->setField(76, 12); // white knight f3
        board->setField(95, 11); // white rook e1

        Tree* tree = new Tree(*board, true, engineOpt, new Hash());
        int eval = tree->alphaBetaExp(*board, 2, 6, 10, NULL, -100000, +100000, true);
        Move best = tree->getBestMove();

        delete tree;
        return;
    }


    void checkRawMoveList() {
        const int size = 120;
        Board* board = new Board();
        board->clearBoard();
        board->setField(95, 15);
        board->setField(25, 25);
        board->setField(85, 10);

        Movegen movegen = Movegen();
        char moves[size] = { '\0'};
        memcpy(moves, movegen.getRawList(board->getBoard(), false, true), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 12); // 6 king and pawm normal moves

        memcpy(moves, movegen.getRawList(board->getBoard(), true, true), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 0); // 0 king and pawn hit moves

        board->setField(76, 20);
        memcpy(moves, movegen.getRawList(board->getBoard(), true, true), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 2); // 1 pawn hit move

        memcpy(moves, movegen.getRawList(board->getBoard(), false, true), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 14); // 7 pawn and king normal moves

        // now black
        memcpy(moves, movegen.getRawList(board->getBoard(), true, false), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 2); // 1 pawn hit move

        memcpy(moves, movegen.getRawList(board->getBoard(), false, false), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 14); // 5 king normal moves, 2 normal pawn moves

        board->clearBoard();
        board->setField(55, 21);
        board->setField(56, 10);
        board->setField(57, 11);
        board->setField(58, 12);

        memcpy(moves, movegen.getRawList(board->getBoard(), true, false), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 2);

        board->setField(56, 10);
        assert (helper::arraySize(moves, size) == 2);

        // tests from init board
        delete board;
        board = new Board();

        memcpy(moves, movegen.getRawList(board->getBoard(), false, true), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 40);

        memcpy(moves, movegen.getRawList(board->getBoard(), false, false), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 40);

        memcpy(moves, movegen.getRawList(board->getBoard(), true, false), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 0);

        memcpy(moves, movegen.getRawList(board->getBoard(), true, true), 120 * sizeof(char));
        assert (helper::arraySize(moves, size) == 0);

        // check complexity
        board = new Board();
        int comp = board->getComplexity();
        assert (comp == 40);

        delete board;
        return;
    }

    /**
     * test if equal bord return 0 in eval value
     */
    void runEvalImpl() {
        Board *board = new Board();
        int ev = Eval().evalMove(board, EvalModus::deep, NULL, this->engineOpt.whitePlayer.evalOpt);
        if (ev != 0) {
            std::cout << "ev is " << ev << std::endl;
        }
        assert(ev == 0);
        return;
    }


    unsigned int getHash32() {
        Hash hash = Hash();
        Board board;
        unsigned int hash32 = hash.get32BitHash(board.getBoard());
        assert (1001487572 == hash32);
        return 0;
    }

    int testEvalImpl() {       
        
        Board *board = new Board();
        // e2-e4 e7-e5
        board->setField(85,  0);
        board->setField(65, 10);
        board->setField(35,  0);
        board->setField(55, 20);
        int evalValue = Eval().evalMove(board, EvalModus::deep, NULL, this->engineOpt.whitePlayer.evalOpt);
        assert (evalValue == 0);
        delete board;
        board = NULL;
        return 0;
    }

    int testDoublePawnEval() {
        Board *board = new Board();
        // e2-d3
        //board->setField(85,  0);
        //board->setField(76,  10);

        board->setField(35,  0);
        board->setField(46,  20);
        int evalValue = Eval().evalMove(board, EvalModus::deep, NULL, this->engineOpt.whitePlayer.evalOpt);
        // std::cout << "Diff Value: " << evalValue << std::endl;
        assert (evalValue > 0);
        delete board;
        board = NULL;
    }
    
    void testGameProgress() {
        // 1st
        assert (this->engineOpt.whitePlayer.evalOpt.sumFigValue == 7800);
        // 2nd
        Board *board = new Board();
        char posList[65] = { '\0' };
        memcpy(posList, Eval().collectFig(board), 64 * sizeof(char));
        int progV = Eval().getProgValue(posList, this->engineOpt.whitePlayer.evalOpt);
        assert (progV == 255);
        // 3th
        board->setField(81, 0);
        posList[65] = { '\0' };
        memcpy(posList, Eval().collectFig(board), 64 * sizeof(char));
        progV = Eval().getProgValue(posList, this->engineOpt.whitePlayer.evalOpt);
        assert (progV < 255);
        return;
    }


    void checkAllowedMove() {

        //check gegenschach
        Board* board = new Board();
        board->clearBoard();
        board->setField(22, 21);
        board->setField(27, 25);
        board->setField(33, 20);
        board->setField(35, 22);
        board->setField(37, 20);
        board->setField(38, 20);
        board->setField(41, 20);
        board->setField(54, 21);
        board->setField(55, 12);
        board->setField(56, 12);
        board->setField(57, 13);
        board->setField(73, 10);
        board->setField(81, 10);
        board->setField(86, 10);
        board->setField(87, 23);
        board->setField(88, 10);
        board->setField(95, 11);
        board->setField(96, 15);
        board->setField(98, 11);
        Tree* tree = new Tree(*board, true, engineOpt, new Hash());
        tree->alphaBetaExp(*board, 1, 4, 12, NULL, -100000, +100000, true);
        Move move = tree->getBestMove();

        delete board;

        // Test für white
        board = new Board();
        board->setField(62, 23); // läufer auf B4
        Move move1 = genTestMove("d2d3", true); // nicht erlaubt
        Move move2 = genTestMove("a2a3", true); // OK
        assert (board->isAllowedMove(move1) == false);
        assert (board->isAllowedMove(move2) == true);
        delete board;
        board = NULL;

        // test for black
        board = new Board();
        board->setField(52, 13); // läufer auf B5
        Move move3 = genTestMove("d7d6", false); // nicht erlaubt
        Move move4 = genTestMove("e7e6", false); // OK
        assert (board->isAllowedMove(move3) == false);
        assert (board->isAllowedMove(move4) == true);
        delete board;
        board = NULL;

        // check for allowed castle
        board = new Board();
        board->setField(96, 0); // delete f1
        board->setField(97, 0); // delete g1
        board->setField(85, 0); // delete e2
        board->setField(63, 23); // schwarz läufer c4
        Move move5 = genTestMove("e1g1", true); // nicht erlaubt
        assert (board->isAllowedMove(move5) == false);
        Move move6 = genTestMove("d2d3", true); // OK
        assert (board->isAllowedMove(move6) == true);
        board->execSingleMoveOnBoard(&move6);
        //Move move7 = genTestMove("e1g1", true); // jetzt OK
        //assert (board->isAllowedMove(move7) == true);
        delete board;
        board = NULL;
        return;
    }

    /**
     * check splits and perform a given std:string move
     * on a init Board
     * Used by Uci-Token 'moves'
     */
    void splitMoveStr() {
        Board* board = new Board();
        std::string bspInput = "d2d4 d7d5 e2e3 b8c6";
        std::vector<std::string> splitedInput = moveHelper::splitUciMoveRow(bspInput);
        board->performMoves(splitedInput, true);
        assert (board->getFigureAtIndex(22) == 0);
        assert (board->getFigureAtIndex(34) == 0);
        assert (board->getFigureAtIndex(43) == 22);
        assert (board->getFigureAtIndex(54) == 20);
        assert (board->getFigureAtIndex(64) == 10);
        assert (board->getFigureAtIndex(75) == 10);
        assert (board->getFigureAtIndex(84) == 0);
        assert (board->getFigureAtIndex(85) == 0);
        delete board;
        board = NULL;
        return;
    }

    /**
     * Checks if castle works when very high eval 
     */
    void castle() {
        // shortcastle white
        engineOpt.whitePlayer.evalOpt.kingCastledShort = 500;
        Board* board = new Board();
        std::string bspInput = "e2e4 e7e6 f1e2 f8e7 g1f3 g8f6";
        std::vector<std::string> splitedInput = moveHelper::splitUciMoveRow(bspInput);
        board->performMoves(splitedInput, true);
        assert (board->getFigureAtIndex(96) == 0);
        assert (board->getFigureAtIndex(97) == 0);
        Tree *tree = new Tree(*board, true, engineOpt, new Hash());
        tree->alphaBetaExp(*board, 1, 2, 2, NULL, -100000, +100000, true);
        Move move = tree->getBestMove();
        auto moveList = tree->getSortedMovesFirstLvl();
        char posList[65] = { '\0' };
        memcpy(posList, Eval().collectFig(board), 64 * sizeof(char));
        int progV = Eval().getProgValue(posList, engineOpt.whitePlayer.evalOpt);
        assert (move.getFrom() == 95);
        assert (move.getTo() == 97);
        board->execSingleMoveOnBoard(&move);
        assert (board->getFigureAtIndex(96) == 11);

        // longcastle black
        engineOpt.blackPlayer.evalOpt.kingCastledLong = 500;
        board = new Board();
        bspInput = "d2d3 d7d6 c1f4 c8e6 b1c3 b8c6 f2f3 d8d7 d1c1";
        splitedInput = moveHelper::splitUciMoveRow(bspInput);
        board->performMoves(splitedInput, true);
        assert (board->getFigureAtIndex(22) == 0);
        assert (board->getFigureAtIndex(23) == 0);
        assert (board->getFigureAtIndex(24) == 0);
        tree = new Tree(*board, false, engineOpt, new Hash());
        tree->alphaBetaExp(*board, 1, 1, 1, NULL, -100000, +100000, false);
        move = tree->getBestMove();
        assert (move.getFrom() == 25);
        assert (move.getTo() == 23);
        board->execSingleMoveOnBoard(&move);
        assert (board->getFigureAtIndex(24) == 21);

        // casteleModus
        board = new Board();
        std::string bspInput0 = "b1c3 e7e5 g1f3 d8f6 c3d5 f6f5 d5c7 e8d8";
        splitedInput = moveHelper::splitUciMoveRow(bspInput0);
        board->performMoves(splitedInput, true);
        assert (board->getWhiteCastleModus() == CastleModus::both);
        assert (board->getBlackCastleModus() == CastleModus::noneC);
        std::string bspInput1 = "c7a8 f8e7 d2d3 e7b4 c2c3 b4a5 c1e3";
        splitedInput = moveHelper::splitUciMoveRow(bspInput1);
        board->performMoves(splitedInput, true);
        std::string bspInput2 = "g8e7 d1a4 b8c6 b2b4 a5b6 a8b6 a7b6 b4b5 e7d5 b5c6 d5c3 e3b6 d8e8 a4a5 d7c6 a5c3 f7f6 e2e4 f5f4 a1b1";
        // Arena:Illegaler Zug!: "bestmove e8g8" (kurze Rochade)
        splitedInput = moveHelper::splitUciMoveRow(bspInput2);
        board->performMoves(splitedInput, false);
        tree = new Tree(*board, false, engineOpt, new Hash());
        tree->alphaBetaExp(*board, 1, 2, 2, NULL, -100000, +100000, false);
        Move best = tree->getBestMove();
        assert (best.getFrom() != 25); // Rochade nicht erlaubt
        assert (board->getWhiteCastleModus() == CastleModus::both);
        return;
    }

    /**
     * Checks the boardfreq hashmap
     */
    void BoardFreq() {
        Board* board = new Board();
        Hash* hash = new Hash();
        int hash32 = hash->get32BitHash(board->getBoard());
        assert (hash->getBoardFreq(hash32) == 0);
        hash->addBoardFreq(*board);
        assert (hash->getBoardFreq(hash32) == 1);
        hash->addBoardFreq(*board);
        assert (hash->getBoardFreq(hash32) == 2);
        hash->reset();
        assert (hash->getBoardFreq(hash32) == 0);
        return;
    }

    /**
     * Check if Queen on Board
     */
    void queenOnbOard() {
        Board* board = new Board();
        assert (board->queenOnBoard());
        board->setField(24, 0);
        assert (board->queenOnBoard());
        board->setField(94, 0);
        assert (!board->queenOnBoard());
        return;
    }

    void avoidPatt() {
        Board* board = new Board();
        board->clearBoard();
        board->setField(58, 25);
        board->setField(34, 10);
        board->setField(45, 14);
        board->setField(63, 10);
        board->setField(72, 12);
        board->setField(77, 10);
        board->setField(81, 10);
        board->setField(82, 10);
        board->setField(86, 15);
        board->setField(88, 10);
        board->setField(91, 11);
        board->setField(98, 11);
        
        Tree* tree = new Tree(*board, true, engineOpt, new Hash());
        tree->alphaBetaExp(*board, 1, 4, 12, NULL, -100000, +100000, true);
        Move move = tree->getBestMove();
        assert (move.getTo() != 24); // do not promote
        delete tree;
        tree = NULL;

        return;
    }


    void checkPromotion() {
        Board* board = new Board();
        std::string bspInput = "d2d4 b8c6 d4d5 c6b4 c2c3 b4a6 c1g5 f7f6 g5e3 c7c6 g1f3 a6c7 d5c6 b7c6 c3c4 c8a6 b1d2 e7e5 a2a3 f8e7 g2g3 d7d5 c4c5 d5d4 e3d4 e5d4 e2e4 a6f1 e1f1 e7c5 d2b3 d8d6 f1g1 c5b6 d1c2 d4d3 c2d2 g8e7 b3a5 f6f5 a5c4 d6e6 c4b6 a7b6 d2f4 c7b5 f4e5 e6e5 f3e5 f5e4 e5c4 e7d5 a1e1 a8a4 e1e4 e8d8 b2b3 d3d2 c4d2 a4e4 d2e4 b5a3 e4g5 d8c8 g5f7 h8f8 f7e5 f8f6 e5g4 f6f5 g4e3 f5e5 g1g2 d5e3 f2e3 e5e3 h1c1 e3b3 c1c6 c8d7 c6c1 b3b1 c1b1 a3b1 h2h4 d7c8 h4h5 b1d2 g3g4 h7h6 g2f2 g7g5 f2e2 d2e4 e2e3 e4f6 e3f3 b6b5 f3g3 b5b4 g3f3 b4b3 f3f2 b3b2 f2g2";
        std::vector<std::string> splitedInput = moveHelper::splitUciMoveRow(bspInput);
        board->performMoves(splitedInput, true);
        Tree* tree = new Tree(*board, false, engineOpt, new Hash());
        tree->alphaBetaExp(*board, 1, 4, 12, NULL, -100000, +100000, false);
        Move move = tree->getBestMove();
        assert (move.getTo() == 92);
        board->execSingleMoveOnBoard(&move);
        assert (board->getFigureAtIndex(92) == 24);

        delete tree;
        tree = new Tree(*board, true, engineOpt, new Hash());
        tree->alphaBetaExp(*board, 1, 4, 12, NULL, -100000, +100000, true);
        move = tree->getBestMove();
        assert (board->isAllowedMove(move));
        
        delete board;
        delete tree;
        return;

    }

};

#endif // USEASSERTS

#endif // UNITTEST
