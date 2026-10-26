
#ifndef WORKUNIT_H
#define WORKUNIT_H

#include "WorkNode.h"

class WorkUnit : public WorkNode {

public:
    WorkUnit(int maxRetries);

    WorkNode* clone();

    void add(WorkNode* workNode);
};

#endif
