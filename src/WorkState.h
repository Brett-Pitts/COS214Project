#ifndef WORKSTATE_H
#define WORKSTATE_H

class WorkNode;

class WorkState{
public:
    virtual ~WorkState();

    virtual void handle(WorkNode* workNode) = 0;
};

#endif
