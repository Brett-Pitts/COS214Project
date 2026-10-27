#include "Employee.h"

Employee::Employee(string name) : EmployeeComponent(), name(name) {}

Employee::Employee(const Employee& other) : EmployeeComponent(other), name(other.name) {}

Employee::~Employee() {}

string Employee::getName() {
	return name;
}

std::string Employee::getKind() {
	return "Employee";
}

void Employee::doWork(WorkUnit* workUnit) {
	EmployeeComponent::doWork(workUnit);
}

void Employee::wrap(EmployeeComponent* component) {
	delete component;
}

EmployeeComponent* Employee::clone() {
	EmployeeComponent* ret = new Employee(this->name);

	return ret;
}
