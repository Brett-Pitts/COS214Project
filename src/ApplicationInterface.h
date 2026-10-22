#ifndef APPLICATIONINTERFACE_H
#define APPLICATIONINTERFACE_H

class ApplicationInterface {

private:
	WorkNode workflowRoot;
	CompanyNode companyRoot;

public:
	void createProject();

	void addSubcomponent();

	void addDepartment();

	void addEmployee();

	Memento createMemento();

	void setMemento(Memento memento);
};

#endif
