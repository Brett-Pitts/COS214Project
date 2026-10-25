#ifndef DEPARTMENT_H
#define DEPARTMENT_H

class Department : CompanyNode {

public:
	std::vector<CompanyNode> children;

	void add(CompanyNode companyNode);

	void remove(CompanyNode companyNode);

	CompanyNode getChild(int arg0);
};

#endif
