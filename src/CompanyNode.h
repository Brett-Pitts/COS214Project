#ifndef COMPANYNODE_H
#define COMPANYNODE_H

class CompanyNode {


public:
	virtual void add(CompanyNode* companyNode)=0;

	virtual void remove(CompanyNode* companyNode)=0;

	virtual CompanyNode* getChild(int n)=0;

	~CompanyNode();
};

#endif
