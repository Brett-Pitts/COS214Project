#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "CompanyNode.h"
#include <vector>

/**
 * @class Department
 * @brief Composite node of the company tree: holds departments and teams.
 *
 * @par Design pattern
 * Composite (Composite). Children are stored as CompanyNode pointers and are
 * owned (and deleted) by the department.
 *
 * @see CompanyNode
 */
class Department : public CompanyNode {
private:

	std::vector<CompanyNode*> children; ///< Child departments and teams, in the order they were added.
public:
	Department(std::string name) :CompanyNode(name) {};
	Department(): CompanyNode(""){};
	/**
	 * @brief Adds a child department or team to the end of the list.
	 *
	 * A null pointer is ignored.
	 *
	 * @param companyNode The child to add. Ownership passes to this department.
	 */
	void add(CompanyNode* companyNode);

	/**
	 * @brief Removes the first occurrence of a child from the list.
	 *
	 * The child is not deleted: it belongs to the caller again. Nothing happens if it
	 * is not a child of this department.
	 *
	 * @param companyNode The child to remove.
	 */
	void remove(CompanyNode* companyNode);

	/**
	 * @brief Gets the nth child.
	 * @param n Zero-based index of the child.
	 * @return The nth child, or nullptr if @p n is out of bounds.
	 */
	CompanyNode* getChild(int n);

	/**
	 * @brief Makes a deep copy of this department and all its children.
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual CompanyNode* clone();

	/** @brief Destroys the department and all its children. */
	~Department();
};

#endif
