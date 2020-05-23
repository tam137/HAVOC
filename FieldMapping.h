#ifndef FIELDS_H_INCLUDED
#define FIELDS_H_INCLUDED

#include <string>
#include <map>
#include <cassert>

namespace FieldMapping {

    const char* getFieldNotation(int fieldId);
    int getFieldIndex(char noti[2]);
    
}

#endif