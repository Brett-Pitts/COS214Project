#include "Department.h"

void Department::add(CompanyNode* companyNode) {
	if(companyNode!=nullptr)
	children.push_back(companyNode);
}

void Department::remove(CompanyNode* companyNode) {
	if(!children.empty())
	{
		for (auto it = children.begin(); it != children.end(); ++it) {
		
			if(*it==companyNode)
			{
				children.erase(it);
				return;
			}
		}
	}
}

CompanyNode* Department::getChild(int n) {
	if(n<0||n>=children.size())
	{
		return nullptr;
	}
	return children.at(n);
}

Department::~Department()
{
	for (auto it = children.begin(); it != children.end(); ++it) {
		
		delete *it;
	}
}
