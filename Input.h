#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <string.h>
#include "Helper.h"
#include "CalcMoveResult.h"
#include "Board.h"
#include "FieldMapping.h"

namespace input {

    CalcMoveResult moveUci (Gamehandler* gh, Board board, std::string moveStr) {
        char from[3] = { moveStr[0], moveStr[1] };
        char to[3] = { moveStr[2], moveStr[3] };
        from[2] = '\0';
        to[2] = '\0';
        int fromInt = FieldMapping::getFieldIndex(from);
        int toInt = FieldMapping::getFieldIndex(to);

        CalcMoveResult moveResult;
        Move move = Move(fromInt, toInt, board.indexHasFigure(toInt), 1, NULL,
            board.getFigureAtIndex(fromInt) / 10 == 1 ? true : false);
        moveResult.setBestMove(move);
        moveResult.setMoveNr(gh->getMoveResults().size() +1);
        return moveResult;
    }


}


#endif