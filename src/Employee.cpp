#include "Employee.h"

Employee::Employee(string name) : EmployeeComponent(), name(name) {
}

Employee::Employee(const Employee& other) : EmployeeComponent(other), name(other.name) {
}

Employee::~Employee() {
}

string Employee::getName() {
	return name;
}

void Employee::doWork(WorkUnit* workUnit) {
	EmployeeComponent::doWork(workUnit);
}

void Employee::wrap(EmployeeComponent* component) {
	delete component;
}

EmployeeComponent* Employee::clone() {
	return new Employee(*this);
}
