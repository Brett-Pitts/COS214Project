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
 * Every node has a name, a parent, a list of outstanding prerequisite names, a
 * retry counter and a current state. A new node starts in the
 * WaitingForPrerequisites state. Calls that depend on the state are passed on to
 * the WorkState object. A node that becomes Completed tells its mediator, so that
 * the nodes waiting on it can be released.
 *
 * @par Design patterns
 * - Composite (Component): Project is the composite and WorkUnit is the leaf.
 * - State (Context): holds a WorkState and delegates advanceState() to it.
 * - Iterator (ConcreteAggregate): createIterator() returns a WorkIterator.
 * - Mediator (Colleague): reports its own completion to its WorkMediator and is
 *   told about completed nodes through update().
 * - Prototype (Prototype): clone() is implemented by Project and WorkUnit.
 *
 * @see Project, WorkUnit, WorkState, WorkMediator
 */
class WorkNode : public Aggregate {
	friend class FileSystem;
private:
	EmployeeComponent* assignee;              ///< The employee last given to reassign(), or nullptr. It is only stored.
	int maxRetries;                           ///< How many times failed work may go back to Ready.
	WorkMediator* mediator;                   ///< The mediator told when this node completes, or nullptr. Not owned.
	WorkNode* parent;                         ///< The parent node, or nullptr for the root. Not owned.
	std::vector<std::string> prerequisites;   ///< Names of the nodes that must still complete first. Names are removed by update().
	int retryCount;                           ///< How many retries have been used. Increased by reset().
	WorkState* state;                         ///< The current state. Owned.

protected:
	/**
	 * @brief Creates a node in the WaitingForPrerequisites state. Only subclasses can do this.
	 * @param maxRetries Number of times failed work may be retried.
	 * @param name Name of the node.
	 */
	WorkNode(int maxRetries, std::string name);

	std::string name; ///< The name of this node.

public:

	/** @brief Virtual destructor. Deletes the current state object. */
	virtual ~WorkNode();

	/** @brief Gets the name of the current state, for example "Ready". */
	std::string getStateName();

	/**
	 * @brief Adds a child node.
	 * @param workNode The child to add. Project takes ownership of it; WorkUnit ignores it.
	 */
	virtual void add(WorkNode* workNode) = 0;

	/**
	 * @brief Gets the names of the prerequisites that are still outstanding.
	 *
	 * A name is removed as soon as the node it refers to completes (see update()), so
	 * the list shrinks to empty over time.
	 */
	const std::vector<std::string>& getPrerequisites() const;

	/**
	 * @brief Adds a prerequisite by name.
	 *
	 * The name is stored as given. It is not checked against the tree and duplicates
	 * are not filtered out.
	 *
	 * @param workName Name of the node that must complete first.
	 */
	void addPrerequisite(std::string workName);

	/**
	 * @brief Moves the node on by letting the current state handle the request.
	 *
	 * Calls WorkState::handle() on the current state, which may or may not change the
	 * state. Does nothing if there is no state. If this call moves the node into
	 * Completed and a mediator is set, the mediator is asked to notify its other
	 * colleagues with this node's name, which releases the nodes that wait on it.
	 */
	void advanceState();

	/**
	 * @brief Checks whether all children are completed.
	 * @return true if every child is Completed. The base version has no children and
	 *         always returns true; Project overrides it.
	 */
	virtual bool allChildrenCompleted();

	/**
	 * @brief Creates an iterator over this node and the nodes below it.
	 *
	 * The iterator is not positioned yet: call first() before using it.
	 *
	 * @return A newly allocated WorkIterator. The caller owns it.
	 */
	WorkIterator* createIterator();

	/**
	 * @brief Gets a child by position.
	 * @param index Zero-based index of the child.
	 * @return The child, or nullptr if there is none. The base version has no children
	 *         and always returns nullptr; Project overrides it.
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

	/**
	 * @brief Not implemented: always throws.
	 * @throws const char* The text "Not yet implemented".
	 */
	void modify();

	/**
	 * @brief Checks whether all prerequisites have been met.
	 *
	 * Completed prerequisites are removed from the list by update(), so this is true
	 * exactly when no prerequisite names are left.
	 *
	 * @return true if there are no outstanding prerequisites.
	 */
	bool prerequisitesCompleted();

	/**
	 * @brief Records a different employee as the assignee. The assignee is only stored.
	 * @param employeeComponent The new assignee. Not owned.
	 */
	void reassign(EmployeeComponent* employeeComponent);

	/**
	 * @brief Removes a child node. The base version does nothing; Project overrides it.
	 * @param workNode The child to remove.
	 */
	virtual void remove(WorkNode* workNode);

	/**
	 * @brief Uses up one retry and sends the node back to Ready.
	 *
	 * Increases the retry count and sets the state to Ready. It does not check
	 * hasRetriesLeft(); the caller (Team::assignWork()) does that first.
	 */
	void reset();

	/**
	 * @brief Sets the mediator this node tells when it completes.
	 * @param mediator The mediator. Not owned, and not passed on to child nodes.
	 */
	void setMediator(WorkMediator* mediator);

	/**
	 * @brief Sets the parent pointer.
	 *
	 * This does not add the node to the parent's children; Project::add() does both.
	 *
	 * @param parent The parent. Not owned.
	 */
	void setParent(WorkNode* parent);

	/**
	 * @brief Replaces the current state.
	 *
	 * The old state object is deleted. A nullptr is ignored. The mediator is not told.
	 *
	 * @param workState The new state. Ownership passes to this node.
	 */
	void setState(WorkState* workState);

	/**
	 * @brief Replaces the current state with the state of the given name.
	 *
	 * The recognised names are WaitingForPrerequisites, Ready, Ongoing, Escalated,
	 * Completed, Failed and Cancelled. Any other name is ignored. The mediator is not
	 * told.
	 *
	 * @param stateName Name of the state, for example "Ready".
	 */
	void setState(std::string stateName);

	/**
	 * @brief Called by the mediator when another node completes.
	 *
	 * Removes completedName from the outstanding prerequisites. If it was not one of
	 * them, nothing else happens. If it was the last one and the node is still in
	 * WaitingForPrerequisites, the node is advanced (which moves it to Ready).
	 *
	 * @param completedName Name of the node that completed.
	 */
	void update(std::string completedName);

	/**
	 * @brief Makes a copy of this node.
	 *
	 * The copy has the same name and retry limit. A Project copy also gets copies of
	 * all its children, and a WorkUnit copy has the same required profession. The
	 * state, prerequisites, retry count, mediator and assignee are not carried over:
	 * the copy starts in WaitingForPrerequisites.
	 *
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual WorkNode* clone() = 0;

	/** @brief Gets the maximum number of retries. */
	int getMaxRetries();

	/** @brief Gets the name of this node. */
	std::string getName();

	/**
	 * @brief Sets the name of this node.
	 *
	 * If this node (or, for a Project, anything below it) already has that name, "1"
	 * is added to the end of the name until it is unused.
	 *
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
