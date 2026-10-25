#ifndef EMPLOYEEPROFESSION_H
#define EMPLOYEEPROFESSION_H

class EmployeeProfession : EmployeeDetail {

private:
	string professionName;
	static vector<string> professionList;

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
