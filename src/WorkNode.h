#ifndef WORKNODE_H
#define WORKNODE_H

#include <vector>
#include <string>
#include "Aggregate.h"

class EmployeeComponent;
class WorkIterator;
class WorkMediator;
class WorkState;
class FileSystem;

class WorkNode : public Aggregate {
	friend class FileSystem;
private:
	EmployeeComponent* assignee;
	int maxRetries;
	WorkMediator* mediator;
	WorkNode* parent;
	std::vector<std::string> prerequisites;
	int retryCount;
	WorkState* state;

protected:
	WorkNode(int maxRetries, std::string name);
	std::string name;

public:
	virtual ~WorkNode();
	std::string getStateName();
	virtual void add(WorkNode* workNode) = 0;

	void addPrerequisite(std::string workName);

	void advanceState();

	virtual bool allChildrenCompleted();

	WorkIterator createIterator();

	virtual WorkNode* getChild(int index);

	WorkNode* getParent();

	bool hasRetriesLeft();

	bool isCompleted();

	bool isOngoing();

	bool isReady();

	void modify();

	bool prerequisitesCompleted();

	void reassign(EmployeeComponent* employeeComponent);

	virtual void remove(WorkNode* workNode);

	void reset();

	void setMediator(WorkMediator* mediator);

	void setParent(WorkNode* parent);

	void setState(WorkState* workState);
	void setState(std::string stateName);

	void update();
	virtual WorkNode* clone() = 0;

	int getMaxRetries();
	std::string getName();
	void setName(std::string name);

	virtual bool hasName(std::string name) = 0;

};

#endif
