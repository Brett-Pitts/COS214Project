#ifndef EMPLOYEEPROFESSION_H
#define EMPLOYEEPROFESSION_H

class EmployeeProfession : EmployeeDetail {

private:
	int professionId;
	static vector<string> professionList;

public:
	static int registerProfession(string name);

	static bool deleteProfession(string name);

	static int getProfessionId(string name);

	void prepare(WorkUnit workUnit);

	void perform(WorkUnit workUnit);

	void finish(WorkUnit workUnit);
};

#endif
