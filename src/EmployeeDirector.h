#ifndef EMPLOYEEDIRECTOR_H
#define EMPLOYEEDIRECTOR_H

class EmployeeDirector {


public:
	EmployeeComponent* constructEmployee();

private:
	boolean addProfession(int id);

	boolean addCompetency(int id);

	boolean finish(string name, boolean isManager = false);
};

#endif
