#ifndef MOVE_CPP_INCLUDED
#define MOVE_CPP_INCLUDED

#include "en_EvalModus.h"
#include "en_TreeModus.h"
#include "en_CastleModus.h"
#include "MoveHelper.h"
#include "FieldMapping.h"

#include <vector>
#include <list>
#include <sstream>
#include <iostream>

class Move {

    private:
    int moveId;
    char from;
    char to;
    char level;
    bool hit;
    int eval;
    bool white;
    unsigned int hash32;
    int prio;
    Move* pre;
    Move* post;
    Move* first;
    std::list<Move*> postList;
    std::string msg;
    std::vector<EvalModus> evalModusList;
    TreeModus treeModus;

    public:


    Move();
    Move(std::string uciMoveStr, bool white);
    Move(int from, int to, bool hit, char level, Move *pre, bool white);
    Move* getMinMaxNode();
    const char* getPgnNot();
    int inMoveDelta(int bestEval, int allowedDelta);
    std::vector<Move*> getMoveRow();
    bool isEvenState();
    std::string toUciString();
    int getMoveId() const;
    void setMoveId(int moveId);
    char getFrom() const;
    char getTo() const;
    void setEval(int eval);
    char getLevel() const;
    Move* getPre();
    int getPreMoveId();
    bool hasPre() const;
    bool hasPost() const;
    Move* getPostMove() const;
    bool isWhite() const;
    void setWhite(bool white);
    bool isHit() const;
    int getEval() const;
    double getEvalD() const;
    int setPost(Move* post);
    int getPostMoveId();
    void setHash32(unsigned int hash32);
    unsigned int gethash32();
    int getMoveHash();
    void setDebugMsg(std::string msg);
    void setDebugMsgO(std::string msg);
    std::string getDebugMsg();
    std::list<Move*> getPostMoveList();
    void addPostMoveToPostList(Move* movePtr);
    void setTreeModus(TreeModus treeModus);
    TreeModus getTreeModus();
    void addEvalModus(EvalModus evalModus);
    EvalModus getEvalModus();
    std::vector<EvalModus> getEvalModusList();
    Move* getFirstMove();
    int getFirstMoveId();
    Move* getLastPostMove();
    void setPrio(int newPrio);
    void addPrio(int val);
    int getPrio();
    bool isEqal(Move *move);

};

#endif