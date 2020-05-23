#ifndef MOVE_CPP_INCLUDED
#define MOVE_CPP_INCLUDED

#include "MoveHelper.h"


/**
 * Splits a uci movestring into veector of strings
 */
std::vector<std::string> moveHelper::splitUciMoveRow(std::string uciMoveRow) {
    std::istringstream is(uciMoveRow);
    std::string buf;
    std::vector<std::string> ret;
    while (getline(is, buf, ' ')) {
        ret.push_back(buf);
    }
    return ret;

}

#endif