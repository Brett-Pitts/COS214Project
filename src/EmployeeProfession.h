#ifndef EMPLOYEEPROFESSION_H
#define EMPLOYEEPROFESSION_H

class EmployeeProfession : EmployeeDetail {

private:
	int professionId;
	static vector<std::string> professionList;

public:
	static int registerProfession(std::string name);

	static bool deleteProfession(std::string name);

	static int getProfessionId(std::string name);

	void prepare(WorkUnit workUnit);

	void perform(WorkUnit workUnit);

	void finish(WorkUnit workUnit);

	EmployeeComponent* clone();
};

#endif
