#ifndef APPLICATIONINTERFACE_H
#define APPLICATIONINTERFACE_H

#include "WorkNode.h"
#include "CompanyNode.h"
#include "MementoType.h"
#include "Memento.h"

class ApplicationInterface {

private:
	WorkNode* workflowRoot;
	CompanyNode* companyRoot;

public:
	void createProject();

	void addSubcomponent();

	void addDepartment();

	void addEmployee();

	Memento* createMemento(MementoType type);

	void setMemento(Memento* memento);
	~ApplicationInterface();
};

#endif
