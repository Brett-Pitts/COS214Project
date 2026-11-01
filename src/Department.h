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
	/**
	 * @brief Creates a department with the given name.
	 * @param name Department name.
	 */
	Department(std::string name) :CompanyNode(name) {};

	/** @brief Creates a department with an empty name. */
	Department(): CompanyNode(""){};

	/**
	 * @brief Adds a child to the end of the list (nulls ignored); the department owns it.
	 * @param companyNode The child to add (now owned).
	 */
	void add(CompanyNode* companyNode);

	/**
	 * @brief Removes a child without deleting it.
	 * @param companyNode The child to remove.
	 */
	void remove(CompanyNode* companyNode);

	/**
	 * @brief Gets the nth child, or nullptr if n is out of range.
	 * @param n Zero-based index.
	 * @return The child, or nullptr.
	 */
	CompanyNode* getChild(int n);

	/**
	 * @brief Makes a deep copy of the department and its children.
	 * @return A new copy; the caller owns it.
	 */
	virtual CompanyNode* clone();

	/** @brief Deletes the department and all its children. */
	~Department();
};

#endif
