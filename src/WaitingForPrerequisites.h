#ifndef WAITINGFORPREREQUISITES_H
#define WAITINGFORPREREQUISITES_H

#include "WorkState.h"

/**
 * @class WaitingForPrerequisites
 * @brief Concrete state: the work node waits for other work to be completed first.
 *
 * Every new node starts here. When a prerequisite completes, WorkNode::update()
 * removes it from the node's list and advances the node once the list is empty.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode, ConcreteWorkMediator
 */
class WaitingForPrerequisites : public WorkState {
public:
	/** @brief Creates the state with the name "WaitingForPrerequisites". */
	WaitingForPrerequisites() :WorkState("WaitingForPrerequisites") {};

	/**
	 * @brief Moves the node to Ready if it has no outstanding prerequisites; otherwise leaves it as it is.
	 * @param workNode The node whose state is being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
