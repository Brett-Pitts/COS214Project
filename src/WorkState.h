#ifndef WORKSTATE_H
#define WORKSTATE_H
#include <string>
class WorkNode;

/**
 * @class WorkState
 * @brief Abstract state of a work node.
 *
 * Each concrete state decides what happens when the node is advanced and
 * which state comes next.
 *
 * @par Design pattern
 * State (State). The concrete states are WaitingForPrerequisites, Ready,
 * Ongoing, Escalated, Completed, Failed and Cancelled. The context is
 * WorkNode.
 *
 * @see WorkNode
 */
class WorkState {
private:
	std::string name; ///< Name of the state.
public:
	/**
	 * @brief Creates a state.
	 * @param name Name of the state.
	 */
	WorkState(std::string name) :name(name) {};

	/** @brief Virtual destructor so concrete states are destroyed correctly. */
	virtual ~WorkState();

	/** @brief Gets the name of the state. */
	virtual std::string getName();

	/**
	 * @brief Handles an advance request: performs the state's action and sets the next state.
	 * @param workNode The node whose state is being advanced.
	 */
	virtual void handle(WorkNode* workNode) = 0;
};

#endif
