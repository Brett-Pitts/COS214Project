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

bool ConcreteEmployeeBuilder::addCompetency(int id) {
	if (employee == nullptr) {
		employee = new EmployeeCompetency(id);
	}
	else {
		employee->wrap(new EmployeeCompetency(id));
	}
	return true;
}

bool ConcreteEmployeeBuilder::addProfession(int id) {
	if (employee == nullptr) {
		employee = new EmployeeProfession(id);
	}
	else {
		employee->wrap(new EmployeeProfession(id));
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
