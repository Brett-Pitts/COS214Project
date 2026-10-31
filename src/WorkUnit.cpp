#include "WorkUnit.h"

WorkUnit::WorkUnit(std::string name, std::string requiredProfession) : WorkNode(3, name), requiredProfession(requiredProfession) {}

std::string WorkUnit::getRequiredProfession() const { return requiredProfession; }

// for gui:
void WorkUnit::setRequiredProfession(std::string profession) { requiredProfession = profession; }


WorkNode* WorkUnit::clone() {
	WorkUnit* copy = new WorkUnit(this->name, this->requiredProfession);
	copy->copyStateFrom(this);
	return copy;
}

bool WorkUnit::hasName(std::string name) {
	return name == this->name;
}



void WorkUnit::add(WorkNode* workNode) {
	return;
}
