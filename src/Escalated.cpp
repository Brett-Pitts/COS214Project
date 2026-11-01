#include "Escalated.h"
#include "Completed.h"
#include "WorkNode.h"

void Escalated::handle(WorkNode* workNode) {
	workNode->setState(new Completed());
}
