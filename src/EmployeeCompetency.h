#ifndef EMPLOYEECOMPETENCY_H
#define EMPLOYEECOMPETENCY_H

class EmployeeCompetency : EmployeeDetail {

private:
	int competencyId;
	static vector<string> competencyList;

public:
	static int registerCompetency(string name);

	static bool deleteCompetency(string name);

	static int getCompetencyId(string name);

	void prepare(WorkUnit workUnit);

	void perform(WorkUnit workUnit);

	void finish(WorkUnit workUnit);

	EmployeeComponent* clone();
};

#endif
