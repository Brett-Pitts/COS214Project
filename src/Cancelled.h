#ifndef CANCELLED_H
#define CANCELLED_H

#include "WorkState.h"

class Cancelled : public WorkState {
public:
	Cancelled() :WorkState("Cancelled") {};
	void handle(WorkNode* workNode) override;
};

#endif
