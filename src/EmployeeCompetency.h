#ifndef EMPLOYEECOMPETENCY_H
#define EMPLOYEECOMPETENCY_H

#include "EmployeeDetail.h"

#include <vector>
class FileSystem;

class EmployeeCompetency : public EmployeeDetail {
	friend class FileSystem;
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
	std::string getKind();
	static int registerCompetency(string name);
	static bool deleteCompetency(string name);
	static int getCompetencyId(string name);
	EmployeeComponent* clone();
};

#endif
