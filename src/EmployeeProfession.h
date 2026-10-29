#ifndef EMPLOYEEPROFESSION_H
#define EMPLOYEEPROFESSION_H

#include "EmployeeDetail.h"

#include <vector>
class FileSystem;


class EmployeeProfession : public EmployeeDetail {
	friend class FileSystem;
private:
	std::string professionName;
	static std::vector<std::string> professionList;

protected:
	void prepare(WorkUnit* workUnit);
	void perform(WorkUnit* workUnit);
	void finish(WorkUnit* workUnit);

public:
	EmployeeProfession(std::string name);
	EmployeeProfession(const EmployeeProfession& other);
	virtual ~EmployeeProfession();
	std::string getKind();
	static std::string registerProfession(std::string name);
	static bool deleteProfession(std::string name);
	static bool hasProfession(std::string name);
	bool compareProfession(std::string professionName);
	EmployeeComponent* clone();
};

#endif
