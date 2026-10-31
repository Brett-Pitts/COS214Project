#ifndef EMPLOYEEDIRECTOR_H
#define EMPLOYEEDIRECTOR_H
 
#include <string>
#include "EmployeeComponent.h"
#include "EmployeeBuilder.h"
 
class EmployeeDirector {
private:
	EmployeeBuilder* builder;
 
	EmployeeComponent* build(std::string name, std::string profession, std::string competency);
 
public:

	EmployeeDirector(EmployeeBuilder* builder);
 
	void setBuilder(EmployeeBuilder* builder);
 
	EmployeeComponent* makeIntern(std::string name, std::string profession);
	EmployeeComponent* makeJunior(std::string name, std::string profession);
	EmployeeComponent* makeSenior(std::string name, std::string profession);
};
 
#endif