
#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <string>

#include "WorkNode.h"

class WorkUnit : public WorkNode {

private:
	std::string name;
	int requiredProfession;

public:

	WorkUnit(std::string name, int requiredProfession = -1);
	std::string getName() const { return name; }
	int getRequiredProfession() const { return requiredProfession; }
	void add(WorkNode* workNode);
	WorkNode* clone();
};

#endif
