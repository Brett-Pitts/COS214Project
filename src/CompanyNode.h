#ifndef COMPANYNODE_H
#define COMPANYNODE_H

#include <string>

class CompanyNode {
protected:
	std::string name;

public:
	// for gui:
	CompanyNode(std::string name) :name(name) {};
	std::string getName() const { return name; }
	void setName(std::string newName) { name = newName; }

	/**
	 * @brief Adds a child node.
	 * @param companyNode The child node.
	 */
	virtual void add(CompanyNode* companyNode) = 0;

	/**
	 * @brief Removes a child node.
	 * @param companyNode The child node.
	 */
	virtual void remove(CompanyNode* companyNode) = 0;

	/**
	 * @brief Gets the nth child.
	 * @param n Zero-based index.
	 * @return The child, or nullptr.
	 */
	virtual CompanyNode* getChild(int n) = 0;

	/**
	 * @brief Makes a deep copy of the node.
	 * @return A new copy; the caller owns it.
	 */
	virtual CompanyNode* clone() = 0;

	/** @brief Virtual destructor. */
	virtual ~CompanyNode();
};

#endif
