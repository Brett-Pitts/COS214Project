#include "WorkUnit.h"

WorkUnit::WorkUnit(std::string name, int requiredProfession = -1) : name(name), requiredProfession(requiredProfession) {}

std::string WorkUnit::getName() const { return name; }
int WorkUnit::getRequiredProfession() const { return requiredProfession; }


WorkNode* WorkUnit::clone() {
	return new WorkUnit(this->name, this->requiredProfession);
}



void WorkUnit::add(WorkNode* workNode) {
	return;
}
