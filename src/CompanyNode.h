#ifndef COMPANYNODE_H
#define COMPANYNODE_H

#include <string>

class CompanyNode {
protected:
	std::string name;

public:
	// for gui:
	std::string getName() const { return name; }
	void setName(std::string newName) { name = newName; }

	virtual void add(CompanyNode* companyNode) = 0;
	virtual void remove(CompanyNode* companyNode) = 0;
	virtual CompanyNode* getChild(int n) = 0;
	virtual CompanyNode* clone() = 0;
	virtual ~CompanyNode();
};

#endif
