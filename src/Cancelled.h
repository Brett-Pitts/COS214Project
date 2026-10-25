#ifndef CANCELLED_H
#define CANCELLED_H

#include "WorkState.h"

class Cancelled : public WorkState{
public:
    void handle(WorkNode* workNode) override;
};

#endif
