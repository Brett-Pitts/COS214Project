#include "WorkUnit.h"

WorkNode* WorkUnit::clone() {
	return new WorkUnit(this->getMaxRetries());
}



void WorkUnit::add(WorkNode* workNode) {

}
