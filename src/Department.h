#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "CompanyNode.h"
#include <vector>
class Department : CompanyNode {

public:
	std::vector<CompanyNode*> children;

	void add(CompanyNode* companyNode);

	//Removes first instance of companyNode
	void remove(CompanyNode* companyNode);

	//Returns the nth child. Returns null if out of bounds
	CompanyNode* getChild(int n);
	
	~Department();
};

#endif
