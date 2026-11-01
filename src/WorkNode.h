#ifndef WORKNODE_H
#define WORKNODE_H

#include <vector>
#include <string>
#include "Aggregate.h"
#include "WorkIterator.h"
#include "WorkMediator.h"
class EmployeeComponent;
class WorkMediator;
class WorkState;
class FileSystem;

/** @brief Base of the work tree. Holds the state, prerequisites and retry count. */
class WorkNode : public Aggregate {
	friend class FileSystem;
private:
	/** The employee assigned to this node. */
	EmployeeComponent* assignee;
	/** Number of retries allowed. */
	int maxRetries;
	/** Used to announce completion. Not owned. */
	WorkMediator* mediator;
	/** The parent node, nullptr at the root. */
	WorkNode* parent;
	/** Names of the nodes that must complete first. */
	std::vector<std::string> prerequisites;
	/** Retries used so far. */
	int retryCount;
	/** The current state. Owned. */
	WorkState* state;

protected:
	/**
	 * @brief Creates a node with a retry limit and a name.
	 * @param maxRetries Retry limit.
	 * @param name Node name.
	 */
	WorkNode(int maxRetries, std::string name);
	/** Name of the node. */
	std::string name;
	
	
public:

	/**
	 * @brief Finds a node by name, or nullptr.
	 * @param name Name to find.
	 * @return The node, or nullptr.
	 */
	virtual WorkNode* getNode(std::string name) = 0;
	/**
	 * @brief Creates a node with a retry limit and a name.
	 * @param maxRetries Retry limit.
	 * @param name Node name.
	 */
	virtual void refresh();
	/**
	 * @brief Gets the root of the tree this node is in.
	 * @return The root node.
	 */
	WorkNode* getRoot();

	/** @brief Deletes the node's state. */
	virtual ~WorkNode();

	/**
	 * @brief Gets the name of the current state.
	 * @return The state name.
	 */
	std::string getStateName();

	/**
	 * @brief Adds a child node (only projects actually do).
	 * @param workNode The child to add.
	 */
	virtual void add(WorkNode* workNode) = 0;

	/**
	 * @brief Gets the names of the prerequisites still outstanding.
	 * @return Names of the outstanding prerequisites.
	 */
	const std::vector<std::string>& getPrerequisites() const;

	/**
	 * @brief Adds a prerequisite by name and puts the node back to waiting.
	 * @param workName Name of the prerequisite.
	 */
	void addPrerequisite(std::string workName);

	/** @brief Advances the state, then tells the mediator if the node just completed. */
	void advanceState();

	/**
	 * @brief True by default; projects check their children.
	 * @return Always true here.
	 */
	virtual bool allChildrenCompleted();

	/**
	 * @brief Creates a WorkIterator over this node's tree.
	 * @return A new WorkIterator; the caller owns it.
	 */
	WorkIterator* createIterator();

	/**
	 * @brief Gets a child by index; nullptr by default.
	 * @param index Zero-based index.
	 * @return The child, or nullptr.
	 */
	virtual WorkNode* getChild(int index);

	/**
	 * @brief Gets the parent, or nullptr at the root.
	 * @return The parent, or nullptr.
	 */
	WorkNode* getParent();

	/**
	 * @brief True if the retry limit hasn't been reached.
	 * @return True if retries remain.
	 */
	bool hasRetriesLeft();

	/**
	 * @brief True if the node is Completed.
	 * @return True if Completed.
	 */
	bool isCompleted();

	/**
	 * @brief True if the node is Ongoing.
	 * @return True if Ongoing.
	 */
	bool isOngoing();

	/**
	 * @brief True if this node or an ancestor still has prerequisites.
	 * @return True if prerequisites are outstanding.
	 */
	bool isWaitingForPrerequisites();

	/**
	 * @brief True if the node is Escalated.
	 * @return True if Escalated.
	 */
	bool isEscalated();

	/**
	 * @brief True if the node is Ready.
	 * @return True if Ready.
	 */
	bool isReady();
	/** @brief Not implemented. Throws if called. */
	void modify();


	/**
	 * @brief Sets the employee assigned to this node.
	 * @param employeeComponent The new assignee.
	 */
	void reassign(EmployeeComponent* employeeComponent);

	/**
	 * @brief Removes a child; does nothing by default.
	 * @param workNode The child to remove.
	 */
	virtual void remove(WorkNode* workNode);

	/** @brief Sends an ongoing or escalated node back to Ready and counts a retry. */
	void reset();

	/**
	 * @brief Sets the mediator used to announce completion.
	 * @param mediator The mediator.
	 */
	void setMediator(WorkMediator* mediator);

	/**
	 * @brief Sets the parent node.
	 * @param parent The parent node.
	 */
	void setParent(WorkNode* parent);

	/**
	 * @brief Swaps in a new state object; null is ignored.
	 * @param workState The new state (now owned, null is ignored).
	 */
	void setState(WorkState* workState);

	/**
	 * @brief Sets the state from its name.
	 * @param stateName Name of the state.
	 */
	void setState(std::string stateName);

	/**
	 * @brief Drops a completed prerequisite and moves to Ready if none are left.
	 * @param completedName Name of the node that completed.
	 */
	virtual void update(std::string completedName);

	/**
	 * @brief Makes a copy of the node (and its children).
	 * @return A new copy; the caller owns it.
	 */
	virtual WorkNode* clone() = 0;

	/**
	 * @brief Gets the retry limit.
	 * @return The retry limit.
	 */
	int getMaxRetries();
	// for gui:
	void setMaxRetries(int retries);
	// for gui:
	void removePrerequisite(std::string workName);

	/**
	 * @brief Gets the node's name.
	 * @return The name.
	 */
	std::string getName();

	/**
	 * @brief Sets the name, adding "1" while it is already taken.
	 * @param name The new name.
	 */
	void setName(std::string name);
	// for gui:
	void copyStateFrom(WorkNode* other);

	/**
	 * @brief True if this node or something below it has the name.
	 * @param name Name to look for.
	 * @return True if the name is found.
	 */
	virtual bool hasName(std::string name) = 0;
};

#endif
