#ifndef PGNHEADER_H_INCLUDED
#define PGNHEADER_H_INCLUDED


#include <fstream>

/**
 * Contains meta information to the chess game
 * Stores in pgn as Meta informations
 * 
 * some Informations, like ply, dynamic writen in Gamehandler
 */


struct PgnHeader {
    std::string Event = "CPU Game";
    std::string Site = "avin1MFPTF2";
    std::string Date = "1970.01.01";
    std::string White;
    std::string Black;
    std::string Ply;
};

#endif

/**
tam137 | ~/workspace/cpp/chess ❥ cat exp2.pgn 
[Event "Computer Schach Partie"]
[Site "avin1MFPTF2"]
[Date "2018.12.10"]
[Round "?"]
[White "jle"]
[Black "jle"]
[Result "*"]
[BlackElo "2400"]
[ECO "A70"]
[Opening "Benoni"]
[Time "20:54:46"]
[WhiteElo "2400"]
[TimeControl "300"]
[Termination "unterminated"]
[PlyCount "123"]
[WhiteType "human"]
[BlackType "human"]
**/