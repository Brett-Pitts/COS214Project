#ifndef CONCRETEWORKMEDIATOR_H
#define CONCRETEWORKMEDIATOR_H

#include "WorkMediator.h"
#include <vector>
#include <string>

class WorkNode;

class ConcreteWorkMediator : public WorkMediator {

public:
	std::vector<WorkNode*> colleagues;

	void notifyColleagues(WorkNode* node, std::string msg);
};

#endif
