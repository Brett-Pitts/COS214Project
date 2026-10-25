#ifndef WORKFLOWMEMENTO_H
#define WORKFLOWMEMENTO_H

class WorkflowMemento : Memento {

public:
	WorkNode workflowRoot;

	WorkNode* getWorkflowRoot();
};

#endif
