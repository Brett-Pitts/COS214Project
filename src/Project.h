
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
  * A project holds sub-projects and work units. It is completed when all of its
  * children are completed: the last child to complete puts the project in
  * Escalated and advances it (see Escalated::handle()), and that in turn can
  * complete the project's own parent. Phases of a larger project are simply
  * Projects as well.
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
	/** @brief The child nodes in the order added, owned by the project. */
	std::vector<WorkNode*> children;

	/**
	 * @brief Creates an empty project with a retry limit and a name.
	 * @param maxRetries Retry limit.
	 * @param name Project name.
	 */
	Project(int maxRetries, std::string name);

	/** @brief Deletes the project and all its children. */
	virtual ~Project();

	/**
	 * @brief Adds a child, renaming it if the name's taken (nulls ignored).
	 * @param workNode The child to add (now owned).
	 */
	void add(WorkNode* workNode);

	/**
	 * @brief Removes a child without deleting it.
	 * @param workNode The child to remove.
	 */
	void remove(WorkNode* workNode);

	/**
	 * @brief Gets a child by index, or nullptr if out of range.
	 * @param index Zero-based index.
	 * @return The child, or nullptr.
	 */
	WorkNode* getChild(int index);

	/**
	 * @brief True if every child is Completed.
	 * @return True if all are done.
	 */
	bool allChildrenCompleted();

	/**
	 * @brief Copies the project and its children, back at the starting state.
	 * @return A new copy; the caller owns it.
	 */
	WorkNode* clone();

	/**
	 * @brief Finds a node by name in this project or below, or nullptr.
	 * @param name Name to find.
	 * @return The node, or nullptr.
	 */
	virtual WorkNode* getNode(std::string name);

	/**
	 * @brief True if this project or anything below it has the name.
	 * @param name Name to look for.
	 * @return True if the name is found.
	 */
	bool hasName(std::string name);

	/**
	 * @brief Drops a completed prerequisite, and escalates the project if all its children are done.
	 * @param completedName Name of the node that completed.
	 */
	void update(std::string completedName);

	/** @brief Re-checks the state of this project and its children. */
	void refresh();
};

#endif
