#ifndef COMPANYNODE_H
#define COMPANYNODE_H

#include <string>

/** @brief Base of the company tree. Departments and teams derive from it. */
class CompanyNode {
protected:
	/** Name of the node. */
	std::string name;

public:
	// for gui:
	CompanyNode(std::string name) :name(name) {};
	/** @brief Gets the name. */
	std::string getName() const { return name; }
	/** @brief Sets the name. */
	void setName(std::string newName) { name = newName; }

	/** @brief Adds a child node. */
	virtual void add(CompanyNode* companyNode) = 0;
	/** @brief Removes a child node. */
	virtual void remove(CompanyNode* companyNode) = 0;
	/** @brief Gets the nth child, or nullptr. */
	virtual CompanyNode* getChild(int n) = 0;
	/** @brief Makes a deep copy. */
	virtual CompanyNode* clone() = 0;
	/** @brief Virtual destructor. */
	virtual ~CompanyNode();
};

#endif
