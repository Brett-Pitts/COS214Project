#ifndef WORKMEDIATOR_H
#define WORKMEDIATOR_H
#include <string>

class WorkNode;

class WorkMediator {


public:
	virtual void notifyColleagues(WorkNode* node, std::string msg) = 0;
};

#endif
