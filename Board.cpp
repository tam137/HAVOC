#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#define USEASSERTS  0

#include "Board.h"

Board::Board() { 
    this->whiteCastle = CastleModus::both;
    this->blackCastle = CastleModus::both;    
}

Board::Board(EvalOpt evalOptW, EvalOpt evalOptB) {
    this->evalOptW = evalOptW;
    this->evalOptB = evalOptB;
    this->whiteCastle = CastleModus::both;
    this->blackCastle = CastleModus::both;
}

Board::Board(EvalOpt evalOptW, EvalOpt evalOptB, int* brett, CastleModus whiteC, CastleModus blackC) {
    this->evalOptW = evalOptW;
    this->evalOptB = evalOptB;
    this->whiteCastle = whiteC;
    this->blackCastle = blackC;
    memcpy(this->brett, brett, 120 * sizeof(int));
}


/**
 * return this Board pointer obj for a given FEN
 * without notize who to move and en passante and move numbers
 */
Board* Board::initBoardForFen(std::string fen) {
    this->clearBoard();
    char board64[64] = { '\0' };
    int ptr = -1;

    for (char c : fen) {
        if (c == 'r') {
            board64[++ptr] = 21;
        }
        else if (c == 'n') {
            board64[++ptr] = 22;
        }
        else if (c == 'b') {
            board64[++ptr] = 23;
        }
        else if (c == 'q') {
            board64[++ptr] = 24;
        }
        else if (c == 'k') {
            board64[++ptr] = 25;
        }
        else if (c == 'p') {
            board64[++ptr] = 20;
        }
        else if (c == 'R') {
            board64[++ptr] = 11;
        }
        else if (c == 'N') {
            board64[++ptr] = 12;
        }
        else if (c == 'B') {
            board64[++ptr] = 13;
        }
        else if (c == 'Q') {
            board64[++ptr] = 14;
        }
        else if (c == 'K') {
            board64[++ptr] = 15;
        }
        else if (c == 'P') {
            board64[++ptr] = 10;
        }
        else if (c >= 49 && c <= 56) {
            int v = c - 48;
            ptr += v;
        }
    }

    // cpy to board rep
    ptr = 19;
    for (int i = 0; i < 64; i++) {
        if (i%8 == 0) ptr += 2;
        this->brett[ptr++] = board64[i];
    }

    return this;
}

/**
 * clears the board
 * reset castle status
 */
void Board::clearBoard() {
    this->whiteCastle = CastleModus::both;
    this->blackCastle = CastleModus::both;
    for (int i = 21; i < 99; i++) {
        if (this->brett[i] > 0) {
            this->brett[i] = 0;
        }
    }
    return;
}

int* Board::getBoard() {
    return this->brett;
}

// only for unittesting
#if USEASSERTS == 1
void Board::setField(int index, int value) {
    this->brett[index] = value;
}
#endif

/**
 * returns true if index of board has a figure
 */
bool Board::indexHasFigure(int index) {
    if (this->brett[index] > 0) { return true; }
    else return false;
}

/**
 * Returns the Fig in Integer for a given index
 */
int Board::getFigureAtIndex(int index) {
    return this->brett[index];
}

/**
 * Returns the Fig Value in centipawns
 */
int Board::getFigValueAtIndex(int index, bool white) {
    if (white) { return helper::mapFigToValue(this->brett[index]%10, evalOptW); }
    else       { return helper::mapFigToValue(this->brett[index]%10, evalOptB); }
}

bool Board::getWhiteCastleModus() {
    return this->whiteCastle;
}

bool Board::getBlackCastleModus() {
    return this->blackCastle;
}

/**
 * prints the binary board to stdout
 */
void Board::printBoard() {
    for (int i = 20; i < 100; i++) {
        std::cout << "\t" << brett[i];
        if (i%10 == 9) std::cout << std::endl;
    }
}

/**
 * performs moves in uci notation
 * 2 param, move row starts with white
 */
void Board::performMoves(std::vector<std::string> moveList, bool white) {
    int whiteMove = white;
    for (std::string move : moveList) {
        this->execSingleMoveOnBoard(new Move(move, whiteMove));
        if (whiteMove) { whiteMove = false; }
        else whiteMove = true;
    }
}

/**
 * Gets a Move Objec
 * Function perform Move on given brett reference
 * Sets casle status
 */
void Board::execSingleMoveOnBoard(Move* move) {
    #if USEASSERTS == 1
    if (!(brett[move->getFrom()] > 0)) {
        std::cout << "From:" << (int)move->getFrom() << std::endl;
        std::cout << "Brett: " << (int)brett[move->getFrom()] << std::endl;
        assert (false);
    }
    #endif

    if (move->getFrom() == 25) {
        int debug = 1;
    }

    // setze rochade status
    if (this->brett[move->getFrom()]%10 == 5) { // king move
        if (move->isWhite()) this->whiteCastle = CastleModus::noneC;
        else this->blackCastle = CastleModus::noneC;
    }

    else if (this->brett[move->getFrom()]%10 == 1 
    && move->isWhite()
    && this->whiteCastle != CastleModus::noneC) { // rook move white
        if (this->brett[move->getFrom()] == 98) {
            if (this->whiteCastle == CastleModus::shortCastle) {
                this->whiteCastle = CastleModus::noneC;
            } else {
                this->whiteCastle = CastleModus::longCastle;
            }
        }
        if (this->brett[move->getFrom()] == 91) {
            if (this->whiteCastle == CastleModus::longCastle) {
                this->whiteCastle = CastleModus::noneC;
            } else {
                this->whiteCastle = CastleModus::shortCastle;
            }
        }
    }

    else if (this->brett[move->getFrom()]%10 == 1
    && !move->isWhite()
    && this->blackCastle != CastleModus::noneC) { // rook move black
        if (this->brett[move->getFrom()] == 28) { // short
            if (this->blackCastle == CastleModus::shortCastle) {
                this->blackCastle = CastleModus::noneC;
            } else {
                this->blackCastle = CastleModus::longCastle;
            }
        }
        if (this->brett[move->getFrom()] == 21) {
            if (this->blackCastle == CastleModus::longCastle) {
                this->blackCastle = CastleModus::noneC;
            } else {
                this->blackCastle = CastleModus::shortCastle;
            }
        }
    }

    // do castlemove
    if ((move->getTo() - move->getFrom() == 2) && brett[move->getFrom()] % 10 == 5) { // short castle
        brett[move->getFrom()+1] = brett[move->getTo()+1];
        brett[move->getTo()+1] = 0;
        //return;
    }
    else if ((move->getFrom() - move->getTo() == 2)  && brett[move->getFrom()] % 10 == 5) { // long castle
        brett[move->getFrom()-1] = brett[move->getTo()-2];
        brett[move->getTo()-2] = 0;
        //return;
    }

    // do normal move
    brett[move->getTo()] = brett[move->getFrom()]; 
    brett[move->getFrom()] = 0;

    // if Promotion, convert to queen
    if ((move->getTo() / 10 == 2 || move->getTo() / 10 == 9) && brett[move->getTo()] %10 == 0) { // pawn on final row
        brett[move->getTo()] = move->isWhite() ? 14 : 24;
    }

    return;
}

/**
 * Deep copy of Board Obj
 */
Board *Board::cpyBoard() {
    Board *board = new Board(this->evalOptW, this->evalOptB, this->brett, this->whiteCastle, this->blackCastle);
    return board;
}

/**
 * Returns the BoardStatus enum
 */
BoardStatus Board::getBoardStatus() {
    int white = 0;
    int black = 0;
    for (int i = 21; i < 99; i++) { // count kings
        if (this->brett[i] == 25) { black++; }
        if (this->brett[i] == 15) { white++; }
    }
    #if USEASSERTS == 1
    assert(black + white == 1 || black + white == 2);
    #endif
    if (black == white) return BoardStatus::normal;
    if (white > black) return BoardStatus::white_has_won;
    if (black > white) return BoardStatus::black_has_won;
}

/**
 * Gets a Move Object and a Board Object
 * Algorith calculates a MoveRow when Move has several pre Moves
 * and returns a new Board Object after execute this Moves on the given Board
 */
Board* Board::returnBoardForMoves(Move* move) {
    Board *bufBoard = this->cpyBoard();
    if (move->getLevel() == 1) {
        bufBoard->execSingleMoveOnBoard(move);
        return bufBoard;
    }

    std::list<Move*> moves;
    Move* current = move;
    moves.push_front(current);

    while (current->hasPre()) {
        current = current->getPre();
        moves.push_front(current);
    }

    for (auto it = moves.begin(); it != moves.end(); it++) {
        bufBoard->execSingleMoveOnBoard(*it);
    }
    #if USEASSERTS == 1
    assert(moves.size() == move->getLevel());
    #endif

    return bufBoard;
}



/**
 * Check if Move is allowed on the given board
 */
bool Board::isAllowedMove (Move move) {

    bool isCastleMove = false;
    char forbFields[3] = { 0, 0, 0};

    // check allowed rochade
    if (this->getFigureAtIndex(move.getFrom())%10 == 5 // yes is kingmove
    && helper::absolut(move.getFrom(), move.getTo()) == 2) { // yes is castlemove

        if (move.isWhite() && this->whiteCastle == CastleModus::noneC) {
            return false;
        }
        if (!move.isWhite() && this->blackCastle == CastleModus::noneC) {
            return false;
        }

        int from = move.getFrom();
        int to = move.getTo();
        if (from < to && move.isWhite()) {
            if (this->whiteCastle == CastleModus::longCastle) {
                return false;
            }
            isCastleMove = true;
            char *p = forbFields;
            *p++ = 95; *p++ = 96; *p++ = 97;
        }
        else if (from > to && move.isWhite()) {
            if (this->whiteCastle == CastleModus::shortCastle) {
                return false;
            }
            isCastleMove = true;
            char *p = forbFields;
            *p++ = 95; *p++ = 94; *p++ = 93;
        }
        else if (from < to && !move.isWhite()) {
            if (this->blackCastle == CastleModus::longCastle) {
                return false;
            }
            isCastleMove = true;
            char *p = forbFields;
            *p++ = 25; *p++ = 26; *p++ = 27;
        }
        else if (from > to && !move.isWhite()) {
            if (this->blackCastle == CastleModus::shortCastle) {
                return false;
            }
            isCastleMove = true;
            char *p = forbFields;
            *p++ = 25; *p++ = 24; *p++ = 23;
        }
    }

    Board* bufBoard = this->cpyBoard();
    bufBoard->execSingleMoveOnBoard(&move);
    std::list<Move> moveList;
    Movegen *movegen = new Movegen();
    if (isCastleMove) {
        moveList = *(movegen->getMovesHit(bufBoard->getBoard(), NULL, !move.isWhite(), true));
    } else {
        moveList = *(movegen->getMovesHit(bufBoard->getBoard(), NULL, !move.isWhite(), false));
    }
    std::list<Move>::iterator it = moveList.begin();        

    // check gamestatus
    while (it != moveList.end()) {
        Move *movePtr = &(*it);

        if (isCastleMove) {
            for (int i = 0; i < 3; i++) {
                int from = movePtr->getFrom();
                int between = (movePtr->getFrom() + movePtr->getTo()) / 2;
                int to = movePtr->getTo();
                if (forbFields[i] == from || forbFields[i] == between || forbFields[i] == to) {
                    return false;
                }
            }
        }
        Board* checkThisB = bufBoard->returnBoardForMoves(movePtr);
        BoardStatus status = checkThisB->getBoardStatus();
        if (status == BoardStatus::black_has_won || status == BoardStatus::white_has_won) {
            return false;
        }
        it++;
    }
    delete bufBoard;
    bufBoard = NULL;
    return true;
}

/**
 * checks if at least one Queen is on the board
 */
bool Board::queenOnBoard() {
    for (int i = 21; i < 99; i++) {
        if (this->brett[i] % 10 == 4) return true;
    }
    return false;
}

/**
 * Returns the Boar Complexity
 * Startposition is 40
 */
int Board::getComplexity() {
    int whiteCount = helper::arraySize(Movegen().getRawList(this->brett, false, true), 256);
    int blackCount = helper::arraySize(Movegen().getRawList(this->brett, false, false), 256);
    return (whiteCount + blackCount) / 2;
}



#endif