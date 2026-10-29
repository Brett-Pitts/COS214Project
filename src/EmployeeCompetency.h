#ifndef EMPLOYEECOMPETENCY_H
#define EMPLOYEECOMPETENCY_H

#include "EmployeeDetail.h"

#include <vector>
class FileSystem;

class EmployeeCompetency : public EmployeeDetail {
	friend class FileSystem;
private:
	std::string competencyName;
	static std::vector<std::string> competencyList;

protected:
	void prepare(WorkUnit* workUnit);
	void perform(WorkUnit* workUnit);
	void finish(WorkUnit* workUnit);

public:
	EmployeeCompetency(std::string name);
	EmployeeCompetency(const EmployeeCompetency& other);
	virtual ~EmployeeCompetency();
	std::string getKind();
	static std::string registerCompetency(std::string name);
	static bool deleteCompetency(std::string name);
	static bool hasCompetency(std::string name);
	EmployeeComponent* clone();
};

#endif
