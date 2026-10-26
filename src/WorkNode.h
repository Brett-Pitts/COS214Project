#ifndef WORKNODE_H
#define WORKNODE_H

#include <vector>
#include "Aggregate.h"

class EmployeeComponent;
class WorkIterator;
class WorkMediator;
class WorkState;

class WorkNode : public Aggregate{

private:
    EmployeeComponent* assignee;
    int maxRetries;
    WorkMediator* mediator;
    WorkNode* parent;
    std::vector<WorkNode*> prerequisites;
    int retryCount;
    WorkState* state;

protected:
    WorkNode(int maxRetries);

public:
    virtual ~WorkNode();

    virtual void add(WorkNode* workNode) = 0;

    void addPrerequisite(WorkNode* workNode);

    void advanceState();

    virtual bool allChildrenCompleted();

    WorkIterator createIterator();

    virtual WorkNode* getChild(int index);

    WorkNode* getParent();

    bool hasRetriesLeft();

    bool isCompleted();

    void modify();

    bool prerequisitesCompleted();

    void reassign(EmployeeComponent* employeeComponent);

    virtual void remove(WorkNode* workNode);

    void reset();

    void setMediator(WorkMediator* mediator);

    void setParent(WorkNode* parent);

    void setState(WorkState* workState);

    void update();
	virtual WorkNode* clone() = 0;

    int getMaxRetries();

};

#endif
