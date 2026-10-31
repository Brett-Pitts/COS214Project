#ifndef WORKNODE_H
#define WORKNODE_H

#include <vector>
#include <string>
#include "Aggregate.h"
#include "WorkIterator.h"

class EmployeeComponent;
class WorkMediator;
class WorkState;
class FileSystem;

/**
 * @file WorkNode.h
 * @brief Declares the component of the work tree.
 */

/**
 * @class WorkNode
 * @brief Common base of projects and work units; holds state, prerequisites and retries.
 *
 * Every node has a name, a parent, a list of prerequisite names and a current
 * state. Calls that depend on the state are passed on to the WorkState object.
 *
 * @par Design patterns
 * - Composite (Component): Project is the composite and WorkUnit is the leaf.
 * - State (Context): holds a WorkState and delegates advanceState() to it.
 * - Iterator (ConcreteAggregate): createIterator() returns a WorkIterator.
 * - Mediator (Colleague): is told about completed nodes through update().
 * - Prototype (Prototype): clone() produces a deep copy.
 *
 * @see Project, WorkUnit, WorkState, WorkMediator
 */
class WorkNode : public Aggregate {
	friend class FileSystem;
private:
	EmployeeComponent* assignee;              ///< The employee currently assigned, or nullptr.
	int maxRetries;                           ///< How many times failed work may go back to Ready.
	WorkMediator* mediator;                   ///< The mediator this node reports to, or nullptr.
	WorkNode* parent;                         ///< The parent node, or nullptr for the root.
	std::vector<std::string> prerequisites;   ///< Names of the nodes that must complete first.
	int retryCount;                           ///< How many retries have been used.
	WorkState* state;                         ///< The current state. Owned.

protected:
	/**
	 * @brief Creates a node. Only subclasses can do this.
	 * @param maxRetries Number of times failed work may be retried.
	 * @param name Name of the node.
	 */
	WorkNode(int maxRetries, std::string name);

	std::string name; ///< The name of this node.

public:

	/** @brief Virtual destructor. */
	virtual ~WorkNode();

	/** @brief Gets the name of the current state, for example "Ready". */
	std::string getStateName();

	/**
	 * @brief Adds a child node.
	 * @param workNode The child to add.
	 */
	virtual void add(WorkNode* workNode) = 0;

	/** @brief Gets the names of the nodes that must complete before this one can start. */
	const std::vector<std::string>& getPrerequisites() const;

	/**
	 * @brief Adds a prerequisite.
	 * @param workName Name of the node that must complete first.
	 */
	void addPrerequisite(std::string workName);

	/** @brief Moves the node on by letting the current state choose the next one. */
	void advanceState();

	/**
	 * @brief Checks whether all children are completed.
	 * @return true if all children are Completed. The base version always has no children.
	 */
	virtual bool allChildrenCompleted();

	/**
	 * @brief Creates an iterator over this node and the nodes below it.
	 * @return A newly allocated WorkIterator. The caller owns it.
	 */
	WorkIterator* createIterator();

	/**
	 * @brief Gets a child by position.
	 * @param index Zero-based index of the child.
	 * @return The child, or nullptr if there is none.
	 */
	virtual WorkNode* getChild(int index);

	/** @brief Gets the parent node, or nullptr if this is the root. */
	WorkNode* getParent();

	/**
	 * @brief Checks whether failed work may still be retried.
	 * @return true if the retry count is below the maximum.
	 */
	bool hasRetriesLeft();

	/** @brief Checks whether the node is in the Completed state. */
	bool isCompleted();

	/** @brief Checks whether the node is in the Ongoing state. */
	bool isOngoing();

	/** @brief Checks whether the node is in the Ready state. */
	bool isReady();

	/** @brief Changes the node's contents or state after it has been worked on. */
	void modify();

	/**
	 * @brief Checks whether every prerequisite has been completed.
	 * @return true if all prerequisites are Completed.
	 */
	bool prerequisitesCompleted();

	/**
	 * @brief Assigns the node to a different employee.
	 * @param employeeComponent The new assignee.
	 */
	void reassign(EmployeeComponent* employeeComponent);

	/**
	 * @brief Removes a child node.
	 * @param workNode The child to remove.
	 */
	virtual void remove(WorkNode* workNode);

	/** @brief Resets the node so the work can be done again. */
	void reset();

	/**
	 * @brief Sets the mediator this node reports to.
	 * @param mediator The mediator.
	 */
	void setMediator(WorkMediator* mediator);

	/**
	 * @brief Sets the parent node.
	 * @param parent The parent.
	 */
	void setParent(WorkNode* parent);

	/**
	 * @brief Replaces the current state.
	 * @param workState The new state. Ownership passes to this node.
	 */
	void setState(WorkState* workState);

	/**
	 * @brief Replaces the current state by name.
	 * @param stateName Name of the state, for example "Ready".
	 */
	void setState(std::string stateName);

	/**
	 * @brief Called by the mediator when another node completes.
	 * @param completedName Name of the node that completed.
	 */
	void update(std::string completedName);

	/**
	 * @brief Makes a deep copy of this node.
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual WorkNode* clone() = 0;

	/** @brief Gets the maximum number of retries. */
	int getMaxRetries();

	/** @brief Gets the name of this node. */
	std::string getName();

	/**
	 * @brief Sets the name of this node.
	 * @param name The new name.
	 */
	void setName(std::string name);

	/**
	 * @brief Checks whether this node (or a node below it) has a given name.
	 * @param name The name to look for.
	 * @return true if a match is found.
	 */
	virtual bool hasName(std::string name) = 0;
};

#endif
