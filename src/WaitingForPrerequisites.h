#ifndef WAITINGFORPREREQUISITES_H
#define WAITINGFORPREREQUISITES_H

#include "WorkState.h"

class WaitingForPrerequisites : public WorkState{
public:
    void handle(WorkNode* workNode) override;
};

#endif
