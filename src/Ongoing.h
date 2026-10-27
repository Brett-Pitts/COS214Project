#ifndef ONGOING_H
#define ONGOING_H

#include "WorkState.h"

class Ongoing : public WorkState {
public:
	Ongoing() :WorkState("Ongoing") {};

	void handle(WorkNode* workNode) override;
};

#endif
