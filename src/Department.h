#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "CompanyNode.h"
#include <vector>
class Department : public CompanyNode {
private:

	std::vector<CompanyNode*> children;
public:

	void add(CompanyNode* companyNode);

	//Removes first instance of companyNode
	void remove(CompanyNode* companyNode);

	//Returns the nth child. Returns null if out of bounds
	CompanyNode* getChild(int n);
	virtual CompanyNode* clone();

	~Department();
};

#endif
