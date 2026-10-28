#ifndef READY_H
#define READY_H

#include "WorkState.h"

class Ready : public WorkState {
public:
	Ready() :WorkState("Ready") {};

	void handle(WorkNode* workNode) override;
};

#endif
