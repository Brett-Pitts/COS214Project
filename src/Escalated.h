#ifndef ESCALATED_H
#define ESCALATED_H

#include "WorkState.h"

class Escalated : public WorkState {
public:
	Escalated() :WorkState("Escalated") {};

	void handle(WorkNode* workNode) override;
};

#endif
