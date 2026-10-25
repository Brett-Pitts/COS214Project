#ifndef EMPLOYEECOMPETENCY_H
#define EMPLOYEECOMPETENCY_H

class EmployeeCompetency : EmployeeDetail {

private:
	string competencyName;
	static vector<string> competencyList;

public:

	void prepare(WorkUnit workUnit);

	void perform(WorkUnit workUnit);

	void finish(WorkUnit workUnit);

	EmployeeComponent* clone();

	static boolean addDetail(string name);

	static boolean hasDetail(string name);

	static boolean deleteDetail(string name);

	static std::vector<std::string>& getDetails();
};

#endif
