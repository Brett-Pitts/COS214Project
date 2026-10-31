#ifndef APPLICATIONINTERFACE_H
#define APPLICATIONINTERFACE_H

#include "WorkNode.h"
#include "CompanyNode.h"
#include "MementoType.h"
class Memento;
class ApplicationInterface {

private:
	WorkNode* workflowRoot;
	CompanyNode* companyRoot;

public:
	ApplicationInterface() : workflowRoot(nullptr), companyRoot(nullptr) {}

	void createProject();

	void addSubcomponent();

	void addDepartment();

	void addEmployee();

	void setWorkflow(WorkNode* root);
	void setCompany(CompanyNode* root);
	Memento* createMemento(MementoType type);
	void setMemento(Memento* memento);


	~ApplicationInterface();
};

#endif
