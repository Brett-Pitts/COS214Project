
#ifndef PROJECT_H
#define PROJECT_H

#include "WorkNode.h"
#include <vector>
#include <string>

class Project : public WorkNode {

public:
	std::vector<WorkNode*> children;

	Project(int maxRetries, std::string name);
	virtual ~Project();

	void add(WorkNode* workNode);

	void remove(WorkNode* workNode);

	WorkNode* getChild(int index);

	bool allChildrenCompleted();

	WorkNode* clone();

	bool hasName(std::string name);
};

#endif
