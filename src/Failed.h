#ifndef FAILED_H
#define FAILED_H

#include "WorkState.h"

class Failed : public WorkState {
public:
	Failed() :WorkState("Failed") {};

	void handle(WorkNode* workNode) override;
};

#endif // FAILED_H
