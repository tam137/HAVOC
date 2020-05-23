#ifndef FIELDS_CPP_INCLUDED
#define FIELDS_CPP_INCLUDED

#include "FieldMapping.h"



const char* FieldMapping::getFieldNotation(int fieldId) {
    switch (fieldId) {
        case 21: return "a8";
        case 22: return "b8";
        case 23: return "c8";
        case 24: return "d8";
        case 25: return "e8";
        case 26: return "f8";
        case 27: return "g8";
        case 28: return "h8";

        case 31: return "a7";
        case 32: return "b7";
        case 33: return "c7";
        case 34: return "d7";
        case 35: return "e7";
        case 36: return "f7";
        case 37: return "g7";
        case 38: return "h7";

        case 41: return "a6";
        case 42: return "b6";
        case 43: return "c6";
        case 44: return "d6";
        case 45: return "e6";
        case 46: return "f6";
        case 47: return "g6";
        case 48: return "h6";

        case 51: return "a5";
        case 52: return "b5";
        case 53: return "c5";
        case 54: return "d5";
        case 55: return "e5";
        case 56: return "f5";
        case 57: return "g5";
        case 58: return "h5";

        case 61: return "a4";
        case 62: return "b4";
        case 63: return "c4";
        case 64: return "d4";
        case 65: return "e4";
        case 66: return "f4";
        case 67: return "g4";
        case 68: return "h4";

        case 71: return "a3";
        case 72: return "b3";
        case 73: return "c3";
        case 74: return "d3";
        case 75: return "e3";
        case 76: return "f3";
        case 77: return "g3";
        case 78: return "h3";

        case 81: return "a2";
        case 82: return "b2";
        case 83: return "c2";
        case 84: return "d2";
        case 85: return "e2";
        case 86: return "f2";
        case 87: return "g2";
        case 88: return "h2";

        case 91: return "a1";
        case 92: return "b1";
        case 93: return "c1";
        case 94: return "d1";
        case 95: return "e1";
        case 96: return "f1";
        case 97: return "g1";
        case 98: return "h1";
        default: assert(false);
    }

}


int FieldMapping::getFieldIndex(char noti[2]) {
    //noti[2] = '\0';
    std::string notistr(noti);
    if (notistr.compare("a8") == 0) return 21;
    if (notistr.compare("b8") == 0) return 22;
    if (notistr.compare("c8") == 0) return 23;
    if (notistr.compare("d8") == 0) return 24;
    if (notistr.compare("e8") == 0) return 25;
    if (notistr.compare("f8") == 0) return 26;
    if (notistr.compare("g8") == 0) return 27;
    if (notistr.compare("h8") == 0) return 28;

    if (notistr.compare("a7") == 0) return 31;
    if (notistr.compare("b7") == 0) return 32;
    if (notistr.compare("c7") == 0) return 33;
    if (notistr.compare("d7") == 0) return 34;
    if (notistr.compare("e7") == 0) return 35;
    if (notistr.compare("f7") == 0) return 36;
    if (notistr.compare("g7") == 0) return 37;
    if (notistr.compare("h7") == 0) return 38;

    if (notistr.compare("a6") == 0) return 41;
    if (notistr.compare("b6") == 0) return 42;
    if (notistr.compare("c6") == 0) return 43;
    if (notistr.compare("d6") == 0) return 44;
    if (notistr.compare("e6") == 0) return 45;
    if (notistr.compare("f6") == 0) return 46;
    if (notistr.compare("g6") == 0) return 47;
    if (notistr.compare("h6") == 0) return 48;

    if (notistr.compare("a5") == 0) return 51;
    if (notistr.compare("b5") == 0) return 52;
    if (notistr.compare("c5") == 0) return 53;
    if (notistr.compare("d5") == 0) return 54;
    if (notistr.compare("e5") == 0) return 55;
    if (notistr.compare("f5") == 0) return 56;
    if (notistr.compare("g5") == 0) return 57;
    if (notistr.compare("h5") == 0) return 58;

    if (notistr.compare("a4") == 0) return 61;
    if (notistr.compare("b4") == 0) return 62;
    if (notistr.compare("c4") == 0) return 63;
    if (notistr.compare("d4") == 0) return 64;
    if (notistr.compare("e4") == 0) return 65;
    if (notistr.compare("f4") == 0) return 66;
    if (notistr.compare("g4") == 0) return 67;
    if (notistr.compare("h4") == 0) return 68;

    if (notistr.compare("a3") == 0) return 71;
    if (notistr.compare("b3") == 0) return 72;
    if (notistr.compare("c3") == 0) return 73;
    if (notistr.compare("d3") == 0) return 74;
    if (notistr.compare("e3") == 0) return 75;
    if (notistr.compare("f3") == 0) return 76;
    if (notistr.compare("g3") == 0) return 77;
    if (notistr.compare("h3") == 0) return 78;

    if (notistr.compare("a2") == 0) return 81;
    if (notistr.compare("b2") == 0) return 82;
    if (notistr.compare("c2") == 0) return 83;
    if (notistr.compare("d2") == 0) return 84;
    if (notistr.compare("e2") == 0) return 85;
    if (notistr.compare("f2") == 0) return 86;
    if (notistr.compare("g2") == 0) return 87;
    if (notistr.compare("h2") == 0) return 88;

    if (notistr.compare("a1") == 0) return 91;
    if (notistr.compare("b1") == 0) return 92;
    if (notistr.compare("c1") == 0) return 93;
    if (notistr.compare("d1") == 0) return 94;
    if (notistr.compare("e1") == 0) return 95;
    if (notistr.compare("f1") == 0) return 96;
    if (notistr.compare("g1") == 0) return 97;
    if (notistr.compare("h1") == 0) return 98;
    return -1;
}


#endif