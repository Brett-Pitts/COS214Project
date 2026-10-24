#ifndef MEMENTO_H
#define MEMENTO_H

class Memento {


public:
	virtual WorkNode* getWorkflowRoot() = 0;

	virtual CompanyNode* getCompanyRoot() = 0;
};

#endif
