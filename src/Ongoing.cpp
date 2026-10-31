#include "Ongoing.h"
#include "Completed.h"
#include "WorkNode.h"

void Ongoing::handle(WorkNode* workNode) {
	if (workNode->allChildrenCompleted()) {
		WorkNode* parent = workNode->getParent();
		workNode->setState(new Completed());

		if (parent != nullptr) {
			parent->advanceState();
		}
	}
}
