#ifndef EMPLOYEECOMPETENCY_H
#define EMPLOYEECOMPETENCY_H

#include "EmployeeDetail.h"

#include <vector>

class EmployeeCompetency : public EmployeeDetail {

private:
	int competencyId;
	static vector<string> competencyList;

protected:
	void prepare(WorkUnit* workUnit);
	void perform(WorkUnit* workUnit);
	void finish(WorkUnit* workUnit);

public:
	EmployeeCompetency(string name);
	EmployeeCompetency(const EmployeeCompetency& other);
	virtual ~EmployeeCompetency();

	static int registerCompetency(string name);
	static bool deleteCompetency(string name);
	static int getCompetencyId(string name);
	EmployeeComponent* clone();
};

#endif
