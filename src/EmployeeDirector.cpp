#include "EmployeeDirector.h"
#include <iostream>

EmployeeDirector::EmployeeDirector(EmployeeBuilder* builder) : builder(builder) {}

void EmployeeDirector::setBuilder(EmployeeBuilder* builder) {

	if (builder == nullptr) {

		std::cerr << "NULL\n";
		return;

	}

	this->builder = builder;

}

EmployeeComponent* EmployeeDirector::build(std::string name, std::string profession, std::string competency) {

	builder->reset();
	builder->addProfession(profession);
	builder->addCompetency(competency);
	return builder->finish(name);
}

EmployeeComponent* EmployeeDirector::makeIntern(std::string name, std::string profession) {

	return build(name, profession, "Intern");

}

EmployeeComponent* EmployeeDirector::makeJunior(std::string name, std::string profession) {

	return build(name, profession, "Junior");

}

EmployeeComponent* EmployeeDirector::makeSenior(std::string name, std::string profession) {

	return build(name, profession, "Senior");
	
}
