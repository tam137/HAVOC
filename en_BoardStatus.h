#ifndef ENBOARDSTATUS_H_INCLUDED
#define ENBOARDSTATUS_H_INCLUDED

enum BoardStatus { 
    normal,
    white_has_won,
    black_has_won,
    white_in_check,
    black_in_check
};

#endif // ENBOARDSTATUS_H_INCLUDED