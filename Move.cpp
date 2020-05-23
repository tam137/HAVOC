#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include "Move.h"

#define USEASSERTS  0

Move::Move() {  };

/**
 * Converts a ucistring to a move obj
 * Return a simple Move Obj
 */
Move::Move(std::string uciMoveStr, bool white) {
    char cFrom[2] = { uciMoveStr[0], uciMoveStr[1] };
    this->from = FieldMapping::getFieldIndex(cFrom);
    char cTo[2] = { uciMoveStr[2], uciMoveStr[3] };
    this->to = FieldMapping::getFieldIndex(cTo);
    this->level = 1;
    this->pre = NULL;
    this->prio = 0;
    this->white = white;
}

/**
 * Move construktor can deal with raw move list
 */
Move::Move(int from, int to, bool hit, char level, Move *pre, bool white) {
    this->moveId = 0;
    this->from = from;
    this->to = to;
    this->hit = hit;
    this->eval = 0;
    this->level = level;
    this->pre = pre;
    this->white = white;
    this->post = NULL;
    this->hash32 = 0;
    this->postList = std::list<Move*>();
    this->prio = 0;
    #if USEASSERTS == 1
    this->evalModusList.push_back(EvalModus::none);
    this->treeModus = TreeModus::unset;
    if (level == 1) {
        this->treeModus = TreeModus::unset;
    }
    #endif
    this->msg = "";
    if (pre != NULL) {
        Move* buf = pre;
        this->first = buf;
        while (buf->hasPre()) {
            buf = buf->getPre();
            this->first = buf;
        }
    } else {
        this->first = NULL;
    }
    
}

/**
 * Returns the best min max PostNode
 */

Move *Move::getMinMaxNode() {
    Move* minMaxNode = *this->postList.begin();
    for (std::list<Move*>::iterator it = this->postList.begin(); it != this->postList.end(); it++) {
        if (this->white) { // is white take min value
            if ((*it)->getEval() < minMaxNode->getEval()) {
                minMaxNode = *it;
            }
        }
        else { // is black take max value
            if ((*it)->getEval() > minMaxNode->getEval()) {
                minMaxNode = *it;
            }   
        }
    }
    return minMaxNode;
}

    /**
 * return a char array of the move to print in term 
 */
const char* Move::getPgnNot() {
    std::string ret = "";
    ret.append(FieldMapping::getFieldNotation(this->from));
    ret.append(this->isHit() ? "x" : "-");
    ret.append(FieldMapping::getFieldNotation(this->to));

    return ret.c_str();
}

/**
 * Check if node is in a delta of given param
 * in relation to the best move 
 */
int Move::inMoveDelta(int bestEval, int allowedDelta) {
    bool isWhite = this->isWhite();
    #if USEASSERTS == 1
    assert (this != NULL);
    assert (this->getLevel() % 2 == 1);
    assert (allowedDelta >= 0);
    #endif
    if (isWhite) {
        return (bestEval - this->eval <= allowedDelta);
    } else {
        return (bestEval - this->eval >= -allowedDelta);
    }
}

/**
 * returns all minmaxpostmoves of the move
 */
std::vector<Move*> Move::getMoveRow() {
    Move* buf = this;
    std::vector<Move*> ret;
    ret.push_back(this);
    while (buf->hasPost()) {
        buf = buf->getPostMove();
        ret.push_back(buf);
    }
return ret;
}

/**
 * checks 3 things:
 * 1 move is a hit move
 * 2 move has no postmoves
 * 3 move has uneven count on direct pre hitmoves
 * --> means expand here only hitmoves to get a even tree
 * --> then return false (-> false means yes expand)
 */
bool Move::isEvenState() {
    int evenCount = 1; // this node makes it 1 if is a hit one, pres ++
    Move* buf = this;
    if (this->hit && this->postList.size() == 0) { // check 1 & 2
        while (buf->hasPre()) {
            buf = buf->getPre();
            if (buf->isHit()) {
                evenCount++;
            }
        }
        if (evenCount % 2 == 1) { // check 3
            return false;
        }
    }
    return true;
}


/**
 * Converts a Move obj to an uci string
 */
std::string Move::toUciString() {
    std::string from = FieldMapping::getFieldNotation(this->from);
    std::string to = FieldMapping::getFieldNotation(this->to);
    return from + to;
}

/*****************************
 * GETTER AND SETTER
 */
int Move::getMoveId() const {
    return this->moveId;
}

void Move::setMoveId(int moveId) {
    this->moveId = moveId;
}

char Move::getFrom() const {
    return this->from;
}

char Move::getTo() const {
    return this->to;
}

void Move::setEval(int eval) {
    this->eval = eval;
}

char Move::getLevel() const {
    return this->level;
}

Move* Move::getPre() {
    return this->pre;
}

int Move::getPreMoveId() {
    if (this->pre == NULL) return 0;
    else return this->pre->moveId;
}

bool Move::hasPre() const {
    return this->pre == NULL ? false : true;
}

bool Move::hasPost() const {
    return this->post == NULL ? false : true;
}

Move* Move::getPostMove() const {
    #if USEASSERTS == 1
    assert (this->hasPost());
    #endif
    return this->post;
}

bool Move::isWhite() const {
    return this->white;
}

void Move::setWhite(bool white) {
    this->white = white;
    return;
}

bool Move::isHit() const {
    return this->hit;
}

int Move::getEval() const {
    return this->eval;
}

double Move::getEvalD() const {
    return this->eval/100.;
}

int Move::setPost(Move* post) {
    this->post = post;
}

int Move::getPostMoveId() {
    if (this->post == NULL) return 0;
    else return this->post->moveId;
}

void Move::setHash32(unsigned int hash32) {
    this->hash32 = hash32;
}

unsigned int Move::gethash32() {
    return this->hash32;
}

int Move::getMoveHash() {
    int hash = 0;
    hash += this->from*100;
    hash += this->to;
    hash += (white ? 10000 : 20000);
    //hash += (this->level * 100000);
    return hash;
}

void Move::setDebugMsg(std::string msg) {
    #if USEASSERTS == 1
    if (this->msg.size() > 0)  { std::cout << ">>>>>> " << this->msg << std::endl; }
    assert (this->msg.size() == 0);
    #endif
    this->msg = msg;
}

// Overwrite Debug Msg
void Move::setDebugMsgO(std::string msg) {
    this->msg = msg;
}

std::string Move::getDebugMsg() {
    return this->msg;
}

std::list<Move*> Move::getPostMoveList() {
    return this->postList;
}

void Move::addPostMoveToPostList(Move* movePtr) {
    this->postList.push_back(movePtr);
}

#if USEASSERTS == 1
void Move::setTreeModus(TreeModus treeModus) {
    this->treeModus = treeModus;
}

TreeModus Move::getTreeModus() {
    return this->treeModus;
}

void Move::addEvalModus(EvalModus evalModus) {
    this->evalModusList.push_back(evalModus);
}

EvalModus Move::getEvalModus() {
    return this->evalModusList[evalModusList.size()-1];
}

std::vector<EvalModus> Move::getEvalModusList() {
    return this->evalModusList;
}
#endif

Move* Move::getFirstMove() {
    return this->first;
}

int Move::getFirstMoveId() {
    return this->getFirstMove() == NULL ? 0 : this->getFirstMove()->getMoveId();
}

Move* Move::getLastPostMove() {
    #if USEASSERTS == 1
    assert (this->postList.size() > 0);
    #endif
    return *(--(this->postList.end()));
}

/**
 * Sets a prio for sorting
 * overwrite the before prio
 */
void Move::setPrio(int newPrio) {
    this->prio = newPrio;
}

/**
 * Ads a prio to the before value
 */
void Move::addPrio(int val) {
    this->prio += val;
}

/**
 * return the prio of the node
 * Adds value of hitting fig, if its a hit move
 */
int Move::getPrio() {
    return this->hit + this->prio;
}

/**
 * is Equal in MoveHash()
 */
bool Move::isEqal(Move *move) {
    return (this->getMoveHash() == move->getMoveHash()) ? true : false;
}



#endif