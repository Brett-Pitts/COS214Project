#ifndef FAILED_H
#define FAILED_H

#include "WorkState.h"

class Failed : public WorkState{
public:
    void handle(WorkNode* workNode) override;
};

#endif // FAILED_H
