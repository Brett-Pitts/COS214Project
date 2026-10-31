
#ifndef PROJECT_H
#define PROJECT_H

#include "WorkNode.h"
#include <vector>
#include <string>

/**
 * @file Project.h
 * @brief Declares the composite node of the work tree.
 */

/**
 * @class Project
 * @brief A project or phase: a work node that contains other work nodes.
 *
 * A project holds sub-projects and work units. It is completed when all of
 * its children are completed, and it then tells its parent. Phases of a larger
 * project are simply Projects as well.
 *
 * @par Design patterns
 * - Composite (Composite): children are WorkNode pointers.
 * - Prototype (ConcretePrototype): clone() copies the project and its children.
 * - Iterator (ConcreteAggregate): createIterator() is inherited from WorkNode.
 *
 * @see WorkNode, WorkUnit
 */
class Project : public WorkNode {

public:
	/** @brief The child nodes, in the order they were added. Owned by the project. */
	std::vector<WorkNode*> children;

	/**
	 * @brief Creates an empty project.
	 * @param maxRetries Number of times failed work may be retried.
	 * @param name Name of the project.
	 */
	Project(int maxRetries, std::string name);

	/** @brief Destroys the project and all its children. */
	~Project();

	/**
	 * @brief Adds a child to the project.
	 * @param workNode The sub-project or work unit to add. Ownership passes to this project.
	 */
	void add(WorkNode* workNode);

	/**
	 * @brief Removes a child from the project.
	 * @param workNode The child to remove.
	 */
	void remove(WorkNode* workNode);

	/**
	 * @brief Gets a child by position.
	 * @param index Zero-based index of the child.
	 * @return The child, or nullptr if @p index is out of bounds.
	 */
	WorkNode* getChild(int index);

	/**
	 * @brief Checks whether every child has been completed.
	 * @return true if all children are in the Completed state.
	 */
	bool allChildrenCompleted();

	/**
	 * @brief Makes a deep copy of this project and all its children.
	 * @return A newly allocated copy. The caller owns it.
	 */
	WorkNode* clone();

	/**
	 * @brief Checks whether this project, or anything below it, has a given name.
	 * @param name The name to look for.
	 * @return true if a match is found.
	 */
	bool hasName(std::string name);
};

#endif
