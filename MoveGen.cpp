#ifndef MOVEGEN_CPP_INCLUDED
#define MOVEGEN_CPP_INCLUDED

#include "MoveGen.h"

#define USEASSERTS  0


Movegen::Movegen() {  }


/**
 * generiert eine Movelist aus rawMoveList
 */
std::list<Move> *Movegen::getMoveList(int* brett, Move* preMove, bool white, bool hitMoves /*= false*/) {
    char movesRaw[256] = { '\0' };
    memcpy(movesRaw, getRawList(brett, hitMoves, white), 256 * sizeof(char));

    std::list<Move> *movesList = new std::list<Move>();
    int rawSize = helper::arraySize(movesRaw, 256);

    for (int i = 0; i < rawSize; i = i+2) {
        Move *genMove = new Move(movesRaw[i], movesRaw[i+1],
            brett[movesRaw[i+1]] == 0 ? 0 : 1, preMove == NULL ? 1 : preMove->getLevel()+1, preMove, white);
        movesList->push_back(*genMove);
        delete genMove;
        genMove = NULL;
    }
    return movesList;
}

/**
 * Funktion für Schlagzüge
 * castle optional, fügt züge der Liste hinzu, wenn zug auf 25 oder 95 endet
 */
std::list<Move> *Movegen::getMovesHit(int brett[], Move *preMove, bool white, bool castle /*= false*/) {
    std::list<Move> *ret = getMoveList(brett, preMove, white, true);

    if (castle) {
        for(std::list<Move>::iterator it = ret->begin(); it != ret->end(); it++) {
            if (it->getTo() == 95 || it->getTo() == 25) ret->push_back(*it);
        }   
    }
    #if USEASSERTS == 1
    for (Move m : *ret) {
        if (!castle) {
            assert (m.isHit() == 1);
        }
    }
    #endif
    return ret;
}

/**
 * Funktion für Ruhezüge
 */
std::list<Move> *Movegen::getMovesQuite(int brett[], Move *preMove, bool white) {
    std::list<Move> *buf = getMoveList(brett, preMove, white);
    std::list<Move> *ret = new std::list<Move>();
    for(std::list<Move>::iterator it = buf->begin(); it != buf->end(); it++) {
        if (!it->isHit()) ret->push_back(*it);
    }
    delete buf;
    buf = NULL;
    return ret;
}

/**
 * Erzeugt char* moveList
 * brett[] das aktuelle Brett
 * hitmoves true, wenn nur Schlagzüge zurückgegeben wrden sollen, false für beide
 * white Weiß am Zug
 */
char* Movegen::getRawList(int brett[], bool hitMoves, bool white) {

    static char move[256];
    for (int i = 0; i <= 256; i++) {
        move[i] = '\0';
    }
    #if USEASSERTS == 1
    assert (move[256] == '\0');
    #endif

    int movePtr = -1;

    for (int i = 21; i < 99; i++) {
        //Königszüge
        if (brett[i] >= 10){ // Wenn Figur gefunden
            if (white){	// Farbe Weiß	
                if (brett[i] == 15){ // Wenn Figur ein Weißer König
                    if (hitMoves) {
                        if (brett[i - 10] / 10L == 2L){ // Wenn vordere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 10);
                        }
                        if (brett[i + 10] / 10L == 2L){ // Wenn hintere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 10);
                        }
                        if (brett[i + 1] / 10L == 2L){ // Wenn rechtes Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 1);
                        }
                        if (brett[i - 1] / 10L == 2L){ // Wenn linkes Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 1);
                        }
                        if (brett[i - 11] / 10L == 2L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 11);
                        }
                        if (brett[i + 11] / 10L == 2L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 11);
                        }
                        if (brett[i + 9] / 10L == 2L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 9);
                        }
                        if (brett[i - 9] / 10L == 2L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 9);
                        }
                    } else {
                       if (brett[i - 10] == 0 || brett[i - 10] / 10L == 2L){ // Wenn vordere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 10);
                        }
                        if (brett[i + 10] == 0 || brett[i + 10] / 10L == 2L){ // Wenn hintere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 10);
                        }
                        if (brett[i + 1] == 0 || brett[i + 1] / 10L == 2L){ // Wenn rechtes Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 1);
                        }
                        if (brett[i - 1] == 0 || brett[i - 1] / 10L == 2L){ // Wenn linkes Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 1);
                        }
                        if (brett[i - 11] == 0 || brett[i - 11] / 10L == 2L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 11);
                        }
                        if (brett[i + 11] == 0 || brett[i + 11] / 10L == 2L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 11);
                        }
                        if (brett[i + 9] == 0 || brett[i + 9] / 10L == 2L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 9);
                        }
                        if (brett[i - 9] == 0 || brett[i - 9] / 10L == 2L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 9);
                        }
                        // teste auf kurze Rochade
                        if (i == 95 && brett[96] == 0 && brett[97] == 0 && brett[98] == 11) {
                            move[++movePtr] = (i);
                            move[++movePtr] = (i+2);
                        } // teste auf lange Rochade
                        if (i == 95 && brett[94] == 0 && brett[93] == 0 && brett[92] == 0 && brett[91] == 11) {
                            move[++movePtr] = (i);
                            move[++movePtr] = (i-2);
                        }
                    }
                }
            }

            if (!white){	// Farbe schwarz	
                if (brett[i] == 25){ // Wenn Figur ein schwarzer König
                    if (hitMoves) {
                        if (brett[i - 10] / 10L == 1L){ // Wenn vordere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 10);
                        }
                        if (brett[i + 10] / 10L == 1L){ // Wenn hintere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 10);
                        }
                        if (brett[i + 1] / 10L == 1L){ // Wenn rechtes Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 1);
                        }
                        if (brett[i - 1] / 10L == 1L){ // Wenn linkes Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 1);
                        }
                        if (brett[i - 11] / 10L == 1L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 11);
                        }
                        if (brett[i + 11] / 10L == 1L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 11);
                        }
                        if (brett[i + 9] / 10L == 1L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 9);
                        }
                        if (brett[i - 9] / 10L == 1L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 9);
                        }
                    } else {
                        if (brett[i - 10] == 0 || brett[i - 10] / 10L == 1L){ // Wenn vordere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 10);
                        }
                        if (brett[i + 10] == 0 || brett[i + 10] / 10L == 1L){ // Wenn hintere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 10);
                        }
                        if (brett[i + 1] == 0 || brett[i + 1] / 10L == 1L){ // Wenn rechtes Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 1);
                        }
                        if (brett[i - 1] == 0 || brett[i - 1] / 10L == 1L){ // Wenn linkes Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 1);
                        }
                        if (brett[i - 11] == 0 || brett[i - 11] / 10L == 1L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 11);
                        }
                        if (brett[i + 11] == 0 || brett[i + 11] / 10L == 1L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 11);
                        }
                        if (brett[i + 9] == 0 || brett[i + 9] / 10L == 1L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 9);
                        }
                        if (brett[i - 9] == 0 || brett[i - 9] / 10L == 1L){ // Wenn diagonal Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 9);
                        }
                        // teste auf kurze Rochade
                        if (i == 25 && brett[26] == 0 && brett[27] == 0 && brett[28] == 21) {
                            move[++movePtr] = (i);
                            move[++movePtr] = (i+2);
                        } // teste auf lange Rochade
                        if (i == 25 && brett[24] == 0 && brett[23] == 0 && brett[22] == 0 && brett[21] == 21) {
                            move[++movePtr] = (i);
                            move[++movePtr] = (i-2);
                        }
                    }
                }
            }

            //Bauernzüge
            if (white){	// Farbe Weiß		
                if (brett[i] == 10){ // Wenn Figur ein Weißer Bauer
                    if (hitMoves) {
                        if (brett[i - 11] / 10L == 2L){ // Wenn Schlagzug möglich (links)
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 11);
                        }
                        if (brett[i - 9] / 10L == 2L){ // Wenn Schlagzug möglich (rechts)
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 9);
                        }
                    } else {
                        if (brett[i - 10] == 0){ // Wenn vordere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 10);
                            if (i / 10l == 8l && brett[i - 20] == 0){ // Wenn doppelschritt zulässig
                                move[++movePtr] = (i);
                                move[++movePtr] = (i - 20);
                            }
                        }
                        if (brett[i - 11] / 10L == 2L){ // Wenn Schlagzug möglich (links)
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 11);
                        }
                        if (brett[i - 9] / 10L == 2L){ // Wenn Schlagzug möglich (rechts)
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 9);
                        }
                    }
                }
            }

            if (!white){ // Farbe schwarz
                if (brett[i] == 20){ // Wenn Figur ein schwarzer Bauer
                    if (hitMoves) {
                        if (brett[i + 11] / 10L == 1L){ // Wenn Schlagzug möglich (links)
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 11);
                        }
                        if (brett[i + 9] / 10L == 1L){ // Wenn Schlagzug möglich (rechts)
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 9);
                        }
                    } else {
                        if (brett[i + 10] == 0){ // Wenn vordere Feld zulässig
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 10);
                            if (i / 10l == 3l && brett[i + 20] == 0){ // Wenn doppelschritt zulässig
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + 20);
                            }
                        }
                        if (brett[i + 11] / 10L == 1L){ // Wenn Schlagzug möglich (links)
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 11);
                        }
                        if (brett[i + 9] / 10L == 1L){ // Wenn Schlagzug möglich (rechts)
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 9);
                        }
                    }
                }
            } // Bauernzüge abgeschlossen


            if (white){	// Farbe Weiß	
                if (brett[i] == 11 || brett[i] == 14){ // Wenn Figur ein Turm oder Dame
                    if (hitMoves) {
                        for (int j = 10; j<90; j += 10){ // Züge nach Norden
                            if (brett[i - j] / 10L == 2L){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i - j);
                                break;
                            }
                            if (brett[i - j] / 10L == 1L || brett[i - j] / 10L == 2L || brett[i - j] == -11) // Wenn Gegner oder Eigene Figur
                                break;															  // oder rand erreicht
                        }
                        for (int j = -10; j>-90; j -= 10){ // Züge nach Süden
                            if (brett[i - j] / 10L == 2L){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i - j);
                                break;
                            }
                            if (brett[i - j] / 10L == 1L || brett[i - j] / 10L == 2L || brett[i - j] == -11) // Wenn Gegner oder Eigene Figur
                                break;																  // oder rand erreicht
                        }
                        for (int j = 1; j<9; j += 1){ // Züge nach Westen
                            if (brett[i + j] / 10L == 2L){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11) // Wenn Gegner oder Eigene Figur
                                break;																  // oder rand erreicht
                        }
                        for (int j = -1; j>-9; j -= 1){ // Züge nach Osten
                            if (brett[i + j] / 10L == 2L){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11) // Wenn Gegner oder Eigene Figur 
                                break;																  // oder rand erreicht
                        }
                    } else {
                        for (int j = 10; j<90; j += 10){ // Züge nach Norden
                            if (brett[i - j] / 10L == 2L || brett[i - j] == 0){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i - j);
                            }
                            if (brett[i - j] / 10L == 1L || brett[i - j] / 10L == 2L || brett[i - j] == -11) // Wenn Gegner oder Eigene Figur
                                break;															  // oder rand erreicht
                        }
                        for (int j = -10; j>-90; j -= 10){ // Züge nach Süden
                            if (brett[i - j] / 10L == 2L || brett[i - j] == 0){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i - j);
                            }
                            if (brett[i - j] / 10L == 1L || brett[i - j] / 10L == 2L || brett[i - j] == -11) // Wenn Gegner oder Eigene Figur
                                break;																  // oder rand erreicht
                        }
                        for (int j = 1; j<9; j += 1){ // Züge nach Westen
                            if (brett[i + j] / 10L == 2L || brett[i + j] == 0){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11) // Wenn Gegner oder Eigene Figur
                                break;																  // oder rand erreicht
                        }
                        for (int j = -1; j>-9; j -= 1){ // Züge nach Osten
                            if (brett[i + j] / 10L == 2L || brett[i + j] == 0){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11) // Wenn Gegner oder Eigene Figur 
                                break;																  // oder rand erreicht
                        }                        
                    }
                } // Horizontal und Vertikale Züge abgeschlossen für Weiß
            }

            if (!white){	// Farbe Schwarz	
                if (brett[i] == 21 || brett[i] == 24){ // Wenn Figur ein Turm oder Dame
                    if (hitMoves) {
                        for (int j = 10; j<90; j += 10){ // Züge nach Norden
                            if (brett[i - j] / 10L == 1L){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i - j);
                                break;
                            }
                            if (brett[i - j] / 10L == 1L || brett[i - j] / 10L == 2L || brett[i - j] == -11) // Wenn Gegner oder Eigene Figur
                                break;															  // oder rand erreicht
                        }
                        for (int j = -10; j>-90; j -= 10){ // Züge nach Süden
                            if (brett[i - j] / 10L == 1L){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i - j);
                                break;
                            }
                            if (brett[i - j] / 10L == 1L || brett[i - j] / 10L == 2L || brett[i - j] == -11) // Wenn Gegner oder Eigene Figur
                                break;																  // oder rand erreicht
                        }
                        for (int j = 1; j<9; j += 1){ // Züge nach Westen
                            if (brett[i + j] / 10L == 1L){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11) // Wenn Gegner oder Eigene Figur
                                break;																  // oder rand erreicht
                        }
                        for (int j = -1; j>-9; j -= 1){ // Züge nach Osten
                            if (brett[i + j] / 10L == 1L){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11) // Wenn Gegner oder Eigene Figur 
                                break;																  // oder rand erreicht
                        }
                    } else {
                        for (int j = 10; j<90; j += 10){ // Züge nach Norden
                            if (brett[i - j] / 10L == 1L || brett[i - j] == 0){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i - j);
                            }
                            if (brett[i - j] / 10L == 1L || brett[i - j] / 10L == 2L || brett[i - j] == -11) // Wenn Gegner oder Eigene Figur
                                break;															  // oder rand erreicht
                        }
                        for (int j = -10; j>-90; j -= 10){ // Züge nach Süden
                            if (brett[i - j] / 10L == 1L || brett[i - j] == 0){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i - j);
                            }
                            if (brett[i - j] / 10L == 1L || brett[i - j] / 10L == 2L || brett[i - j] == -11) // Wenn Gegner oder Eigene Figur
                                break;																  // oder rand erreicht
                        }
                        for (int j = 1; j<9; j += 1){ // Züge nach Westen
                            if (brett[i + j] / 10L == 1L || brett[i + j] == 0){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11) // Wenn Gegner oder Eigene Figur
                                break;																  // oder rand erreicht
                        }
                        for (int j = -1; j>-9; j -= 1){ // Züge nach Osten
                            if (brett[i + j] / 10L == 1L || brett[i + j] == 0){ // Wenn Feld Frei oder Gegner
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11) // Wenn Gegner oder Eigene Figur 
                                break;																  // oder rand erreicht
                        }
                    }
                }
            } // alle horizontalen Züge abgeschlossen


            if (white){	// Farbe Weiß
                if (brett[i] == 12){	// Wenn Fugur ein Weißer Springer
                    if (hitMoves) {
                        if (brett[i + 21] / 10L == 2L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 21);
                        }
                        if (brett[i + 19] / 10L == 2L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 19);
                        }
                        if (brett[i + 12] / 10L == 2L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 12);
                        }
                        if (brett[i + 8] / 10L == 2L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 8);
                        }
                        if (brett[i - 8] / 10L == 2L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 8);
                        }
                        if (brett[i - 12] / 10L == 2L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 12);
                        }
                        if (brett[i - 19] / 10L == 2L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 19);
                        }
                        if (brett[i - 21] / 10L == 2L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 21);
                        }
                    } else {
                        if (brett[i + 21] / 10L == 2L || brett[i + 21] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 21);
                        }
                        if (brett[i + 19] / 10L == 2L || brett[i + 19] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 19);
                        }
                        if (brett[i + 12] / 10L == 2L || brett[i + 12] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 12);
                        }
                        if (brett[i + 8] / 10L == 2L || brett[i + 8] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 8);
                        }
                        if (brett[i - 8] / 10L == 2L || brett[i - 8] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 8);
                        }
                        if (brett[i - 12] / 10L == 2L || brett[i - 12] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 12);
                        }
                        if (brett[i - 19] / 10L == 2L || brett[i - 19] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 19);
                        }
                        if (brett[i - 21] / 10L == 2L || brett[i - 21] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 21);
                        }
                    }
                }
            }

            if (!white){	// Farbe Schwarz
                if (brett[i] == 22){	// Wenn Fugur ein schwarzer Springer
                    if (hitMoves) {
                        if (brett[i + 21] / 10L == 1L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 21);
                        }
                        if (brett[i + 19] / 10L == 1L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 19);
                        }
                        if (brett[i + 12] / 10L == 1L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 12);
                        }
                        if (brett[i + 8] / 10L == 1L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 8);
                        }
                        if (brett[i - 8] / 10L == 1L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 8);
                        }
                        if (brett[i - 12] / 10L == 1L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 12);
                        }
                        if (brett[i - 19] / 10L == 1L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 19);
                        }
                        if (brett[i - 21] / 10L == 1L){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 21);
                        }
                    } else {
                        if (brett[i + 21] / 10L == 1L || brett[i + 21] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 21);
                        }
                        if (brett[i + 19] / 10L == 1L || brett[i + 19] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 19);
                        }
                        if (brett[i + 12] / 10L == 1L || brett[i + 12] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 12);
                        }
                        if (brett[i + 8] / 10L == 1L || brett[i + 8] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i + 8);
                        }
                        if (brett[i - 8] / 10L == 1L || brett[i - 8] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 8);
                        }
                        if (brett[i - 12] / 10L == 1L || brett[i - 12] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 12);
                        }
                        if (brett[i - 19] / 10L == 1L || brett[i - 19] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 19);
                        }
                        if (brett[i - 21] / 10L == 1L || brett[i - 21] == 0){
                            move[++movePtr] = (i);
                            move[++movePtr] = (i - 21);
                        }
                    }
                }
            } // alle Springer Züger abgeschlossen

            if (white){	// Farbe Weiß
                if (brett[i] == 13 || brett[i] == 14){ // Diagonale Züge
                    if (hitMoves) {
                        for (int j = 9; j<90; j += 9){
                            if (brett[i + j] / 10L == 2L){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = -9; j>-90; j -= 9){
                            if (brett[i + j] / 10L == 2L){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = 11; j<90; j += 11){
                            if (brett[i + j] / 10L == 2L){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = -11; j>-90; j -= 11){
                            if (brett[i + j] / 10L == 2L){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                    } else {
                        for (int j = 9; j<90; j += 9){
                            if (brett[i + j] / 10L == 2L || brett[i + j] == 0){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = -9; j>-90; j -= 9){
                            if (brett[i + j] / 10L == 2L || brett[i + j] == 0){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = 11; j<90; j += 11){
                            if (brett[i + j] / 10L == 2L || brett[i + j] == 0){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = -11; j>-90; j -= 11){
                            if (brett[i + j] / 10L == 2L || brett[i + j] == 0){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                    }
                }
            }// diagonale Züge für Weiß abgeschlossen

            if (!white){	// Farbe schwarz
                if (brett[i] == 23 || brett[i] == 24){ // Diagonale Züge
                    if (hitMoves) {
                        for (int j = 9; j<90; j += 9){
                            if (brett[i + j] / 10L == 1L){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = -9; j>-90; j -= 9){
                            if (brett[i + j] / 10L == 1L){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = 11; j<90; j += 11){
                            if (brett[i + j] / 10L == 1L){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = -11; j>-90; j -= 11){
                            if (brett[i + j] / 10L == 1L){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                                break;
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                    } else {
                        for (int j = 9; j<90; j += 9){
                            if (brett[i + j] / 10L == 1L || brett[i + j] == 0){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = -9; j>-90; j -= 9){
                            if (brett[i + j] / 10L == 1L || brett[i + j] == 0){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = 11; j<90; j += 11){
                            if (brett[i + j] / 10L == 1L || brett[i + j] == 0){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                        for (int j = -11; j>-90; j -= 11){
                            if (brett[i + j] / 10L == 1L || brett[i + j] == 0){
                                move[++movePtr] = (i);
                                move[++movePtr] = (i + j);
                            }
                            if (brett[i + j] / 10L == 1L || brett[i + j] / 10L == 2L || brett[i + j] == -11)
                                break;
                        }
                    }
                }
            }
        }
    }
    return move;
}

#endif