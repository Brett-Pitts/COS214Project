#ifndef WAITINGFORPREREQUISITES_H
#define WAITINGFORPREREQUISITES_H

#include "WorkState.h"

class WaitingForPrerequisites : public WorkState {
public:
	WaitingForPrerequisites() :WorkState("WaitingForPrerequisites") {};

	void handle(WorkNode* workNode) override;
};

#endif
