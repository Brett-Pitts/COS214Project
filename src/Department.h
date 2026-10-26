#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "CompanyNode.h"
#include <vector>
class Department : CompanyNode {

public:
	std::vector<CompanyNode> children;

	void add(CompanyNode* companyNode);

	void remove(CompanyNode* companyNode);

	CompanyNode getChild(int arg0);
};

#endif
