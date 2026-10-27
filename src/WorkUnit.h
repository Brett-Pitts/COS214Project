
#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <string>

#include "WorkNode.h"

class WorkUnit : public WorkNode {

private:

	int requiredProfession;

public:

	WorkUnit(std::string name, int requiredProfession = -1);

	int getRequiredProfession() const { return requiredProfession; }
	void add(WorkNode* workNode);
	WorkNode* clone();
	bool hasName(std::string name);

};

#endif
