#include "ConcreteEmployeeBuilder.h"
#include "EmployeeCompetency.h"
#include "EmployeeProfession.h"
#include "Employee.h"
#include "Manager.h"

void ConcreteEmployeeBuilder::reset() {
	if (employee != nullptr) {
		delete employee;
		employee = nullptr;
	}

}

bool ConcreteEmployeeBuilder::addCompetency(std::string name) {
	if (EmployeeCompetency::getCompetencyLevel(name) < 0) {
		return false;
	}
	if (employee == nullptr) {
		employee = new EmployeeCompetency(EmployeeCompetency::getCompetencyLevel(name));
	}
	else {
		employee->wrap(new EmployeeCompetency(EmployeeCompetency::getCompetencyLevel(name)));
	}
	return true;
}

bool ConcreteEmployeeBuilder::addProfession(std::string name) {
	if (employee == nullptr) {
		employee = new EmployeeProfession(name);
	}
	else {
		employee->wrap(new EmployeeProfession(name));
	}
	return true;
}



EmployeeComponent* ConcreteEmployeeBuilder::finish(std::string name, bool isManager) {
	if (isManager) {

		if (employee == nullptr) {
			employee = new Manager(name);
		}
		else {
			employee->wrap(new Manager(name));
		}
	}
	else {
		if (employee == nullptr) {
			employee = new Employee(name);
		}
		else {
			employee->wrap(new Employee(name));
		}
	}
	EmployeeComponent* temp = employee;
	employee = nullptr;
	return temp;
}
