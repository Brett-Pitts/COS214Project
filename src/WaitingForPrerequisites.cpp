#include "WaitingForPrerequisites.h"
#include "Ready.h"
#include "WorkNode.h"

void WaitingForPrerequisites::handle(WorkNode* workNode) {
	if (!(workNode->isWaitingForPrerequisites())) {
		workNode->setState(new Ready());
	}
}
