#ifndef ONGOING_H
#define ONGOING_H

#include "WorkState.h"

class Ongoing : public WorkState{
public:
    void handle(WorkNode* workNode) override;
};

#endif
