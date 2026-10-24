#ifndef PROJECT_H
#define PROJECT_H

class Project : WorkNode {

public:
	std::vector<WorkNode> children;

	void add(WorkNode workNode);

	void remove(WorkNode workNode);

	WorkNode getChild(int arg0);

	WorkNode* clone();
};

#endif
