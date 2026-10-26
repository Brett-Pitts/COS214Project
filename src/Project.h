
#ifndef PROJECT_H
#define PROJECT_H

#include "WorkNode.h"
#include <vector>

class Project : public WorkNode {

public:
	std::vector<WorkNode*> children;

	Project(int maxRetries);
	~Project();

	void add(WorkNode* workNode);

	void remove(WorkNode* workNode);

	WorkNode* getChild(int index);

	bool allChildrenCompleted();

	WorkNode* clone();
};

#endif
