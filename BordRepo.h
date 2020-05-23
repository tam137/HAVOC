#ifndef BOARDREPO_H_INCLUDED
#define BOARDREPO_H_INCLUDED

#include "Board.h"

int brett_rimKnight[120] = {
		//I
		-11, -11, -11, -11, -11, -11, -11, -11, -11, -11,//n
		-11, -11, -11, -11, -11, -11, -11, -11, -11, -11,//d
		//e
		//   a   b   c   d   e   f   g   h			//x
		-11, 21, 22, 23, 24, 25, 23, 22, 21, -11,//20 - 8
		-11, 20, 20, 20, 20, 20, 20, 20, 20, -11,//30 - 7
		-11, 0, 0, 0, 0, 0, 0, 0, 0, -11,//40 - 6
		-11, 0, 0, 0, 0, 0, 0, 0, 0, -11,//50 - 5
		-11, 0, 0, 0, 0, 0, 0, 0, 0, -11,//60 - 4
		-11, 0, 0, 0, 0, 0, 0, 0, 0, -11,//70 - 3
		-11, 10, 10, 10, 10, 10, 10, 10, 10, -11,//80 - 2
		-11, 11, 12, 13, 14, 15, 13, 12, 11, -11,//90 - 1
		//       1  2  3  4  5  6  7  8 <- Indexbezeichnungen

		-11, -11, -11, -11, -11, -11, -11, -11, -11, -11,
		-11, -11, -11, -11, -11, -11, -11, -11, -11, -11
	};

Board *board_rimKnight = new Board(brett_rimKnight);

#endif