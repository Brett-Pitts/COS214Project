#include "ApplicationInterface.h"

void ApplicationInterface::createProject() {
	// TODO - implement ApplicationInterface::createProject
	throw "Not yet implemented";
}

void ApplicationInterface::addSubcomponent() {
	// TODO - implement ApplicationInterface::addSubcomponent
	throw "Not yet implemented";
}

void ApplicationInterface::addDepartment() {
	// TODO - implement ApplicationInterface::addDepartment
	throw "Not yet implemented";
}

void ApplicationInterface::addEmployee() {
	// TODO - implement ApplicationInterface::addEmployee
	throw "Not yet implemented";
}

Memento* ApplicationInterface::createMemento(MementoType type) {
	Memento* ret = new Memento(nullptr, MementoType::company);
	switch (type) {
	case MementoType::company:
		ret->setRoot(companyRoot->clone(), MementoType::company);
		break;
	case MementoType::workflow:
		ret->setRoot(workflowRoot->clone(), MementoType::workflow);

		break;
	default:
		return nullptr;
	}

	return ret;
}

void ApplicationInterface::setMemento(Memento* memento) {
	switch (memento->getType()) {
	case MementoType::company:
		delete companyRoot;
		companyRoot = ((CompanyNode*)memento->getRoot())->clone();
		break;
	case MementoType::workflow:
		delete workflowRoot;
		workflowRoot = ((WorkNode*)memento->getRoot())->clone();

		break;
	default:
		break;
	}
}

ApplicationInterface::~ApplicationInterface() {
	delete workflowRoot;
	delete companyRoot;
}
