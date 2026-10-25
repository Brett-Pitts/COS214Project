#ifndef READY_H
#define READY_H

#include "WorkState.h"

class Ready : public WorkState{
public:
    void handle(WorkNode* workNode) override;
};

#endif
