#ifndef EMPLOYEEPROFESSION_H
#define EMPLOYEEPROFESSION_H

#include "EmployeeDetail.h"

#include <vector>

class EmployeeProfession : public EmployeeDetail {

private:
	int professionId;
	static vector<string> professionList;

protected:
	void prepare(WorkUnit* workUnit);
	void perform(WorkUnit* workUnit);
	void finish(WorkUnit* workUnit);

public:
	EmployeeProfession(string name);
	EmployeeProfession(const EmployeeProfession& other);
	virtual ~EmployeeProfession();

	static int registerProfession(string name);
	static bool deleteProfession(string name);
	static int getProfessionId(string name);
	bool compareProfession(int professionId);
	EmployeeComponent* clone();
};

#endif
