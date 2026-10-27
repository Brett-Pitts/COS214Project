#include "WorkUnit.h"

WorkUnit::WorkUnit(std::string name, int requiredProfession = -1) : WorkNode(3, name), requiredProfession(requiredProfession) {}

int WorkUnit::getRequiredProfession() const { return requiredProfession; }


WorkNode* WorkUnit::clone() {
	return new WorkUnit(this->name, this->requiredProfession);
}

bool WorkUnit::hasName(std::string name) {
	return name == this->name;
}



void WorkUnit::add(WorkNode* workNode) {
	return;
}
