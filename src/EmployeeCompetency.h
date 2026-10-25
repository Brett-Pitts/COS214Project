#ifndef EMPLOYEECOMPETENCY_H
#define EMPLOYEECOMPETENCY_H

class EmployeeCompetency : EmployeeDetail {

private:
	int competencyId;
	static vector<std::string> competencyList;

public:
	static int registerCompetency(std::string name);

	static bool deleteCompetency(std::string name);

	static int getCompetencyId(std::string name);

	void prepare(WorkUnit workUnit);

	void perform(WorkUnit workUnit);

	void finish(WorkUnit workUnit);

	EmployeeComponent* clone();
};

#endif
