#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <string>

using namespace std;

class EmployeeComponent;

class Mediator {

public:
	virtual ~Mediator() {}
	virtual void notifyColleagues(EmployeeComponent* employeeComponent, string message) = 0;
};

#endif
