#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include <vector>
#include <sstream>

#include "FieldMapping.h"

namespace moveHelper {

    std::vector<std::string> splitUciMoveRow(std::string uciMoveRow);
}

#endif