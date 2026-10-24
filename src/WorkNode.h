#ifndef WORKNODE_H
#define WORKNODE_H

class WorkNode : Aggregate {

private:
	EmployeeComponent assignee;
	int maxRetries;
	WorkMediator mediator;
	WorkNode parent;
	std::vector<WorkNode> prerequisites;
	int retryCount;
	WorkState state;

public:
	void add(WorkNode workNode);

	void addPrerequisite(WorkNode workNode);

	void advanceState();

	boolean allChildrenCompleted();

	WorkIterator createIterator();

	WorkNode getChild(int arg0);

	WorkNode getParent();

	boolean hasRetriesLeft();

	void modify();

	void reassign(EmployeeComponent employeeComponent);

	void remove(WorkNode workNode);

	void reset();

	void setMediator(int ConcreteWorkMediator);

	void setState(WorkState workState);

	void update();
};

#endif
