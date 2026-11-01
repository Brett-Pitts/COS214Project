#include "Employee.h"

Employee::Employee(std::string name) : EmployeeComponent(), name(name) {}

Employee::Employee(const Employee& other) : EmployeeComponent(other), name(other.name) {}

Employee::~Employee() {}

std::string Employee::getName() {
	return name;
}

std::string Employee::getKind() {
	return "Employee";
}

bool Employee::compareCompetency(int requiredCompetency) {
	return false;
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

bool Employee::compareProfession(std::string professionName) {
	return false;
}
