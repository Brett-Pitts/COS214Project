
#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <string>

#include "WorkNode.h"

class WorkUnit : public WorkNode {

private:

	std::string requiredProfession;

public:

	WorkUnit(std::string name, std::string requiredProfession = "");

	std::string getRequiredProfession() const;
	void add(WorkNode* workNode);
	WorkNode* clone();
	bool hasName(std::string name);

};

#endif
