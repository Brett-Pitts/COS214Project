#ifndef WORKUNIT_H
#define WORKUNIT_H

class WorkUnit : WorkNode {


public:
	WorkNode* clone();

	void add(WorkNode workNode);
};

#endif
