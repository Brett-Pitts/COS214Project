#ifndef COMPLETED_H
#define COMPLETED_H

#include "WorkState.h"

class Completed : public WorkState{
public:
    void handle(WorkNode* workNode) override;
};

#endif
