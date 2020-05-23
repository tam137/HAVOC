#ifndef PGNEXPORT_H_INCLUDED
#define PGNEXPORT_H_INCLUDED

/**
 * Class provided pgn export to .pgn file
 */

#include "PgnHeader.h"

class PgnExport {

private:
    std::string pgnContent_string;
    char pgnContent_char[1200] = { '\0' };
    char filename[120]  = { '\0' };
    std::vector<CalcMoveResult> moveResults;
    PgnHeader pgnHeader;


        void genPgnContent() {
        std::string buf;

        buf.append("[Event \"" + this->pgnHeader.Event + "\"]\n");
        buf.append("[Date \"" + this->pgnHeader.Date + "\"]\n");
        buf.append("[Site \"" + this->pgnHeader.Site + "\"]\n");
        buf.append("[White \"" + this->pgnHeader.White + "\"]\n");
        buf.append("[Black \"" + this->pgnHeader.Black + "\"]\n");
        buf.append("[Ply \"" + this->pgnHeader.Ply + "\"]\n");
        buf.append("\n\n");

        for (int i = 0; i < this->moveResults.size(); i++) {
            if (i>0) buf.append(" ");
            if ((i+2)%2 == 0) { 
                buf.append(std::to_string((i+2)/2));
                buf.append(". ");
            }
            buf.append(this->moveResults.at(i).getBestMove().getPgnNot());
        }
        int length = buf.length();
        char res[length+1];
        strcpy(this->pgnContent_char, buf.c_str());
        this->pgnContent_string = buf;
    }

public:
    PgnExport (char const filename[], std::vector<CalcMoveResult> moveResults, PgnHeader pgnHeader) {
        strcpy(this->filename, filename);
        this->moveResults = moveResults;
        this->pgnHeader = pgnHeader;
    }

    void writeGameToPgn() {
        this->genPgnContent();
        std::ofstream myfile;
        myfile.open (this->filename);
        myfile << this->pgnContent_char;
        myfile.close();
    }

};

#endif