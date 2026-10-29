#ifndef EMPLOYEECOMPETENCY_H
#define EMPLOYEECOMPETENCY_H

#include "EmployeeDetail.h"

class FileSystem;

class EmployeeCompetency : public EmployeeDetail {
	friend class FileSystem;
private:
	int competencyId;
	string competencyName;

protected:
	void prepare(WorkUnit* workUnit);
	void perform(WorkUnit* workUnit);
	void finish(WorkUnit* workUnit);

public:
	enum { INTERN = 0, JUNIOR = 1, SENIOR = 2 };

	EmployeeCompetency(string name);
	EmployeeCompetency(const EmployeeCompetency& other);
	virtual ~EmployeeCompetency();

	std::string getKind();
	static int getCompetencyLevel(string name);
	bool compareCompetency(int requiredCompetency);
	EmployeeComponent* clone();
};

#endif
