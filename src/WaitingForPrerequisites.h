#ifndef WAITINGFORPREREQUISITES_H
#define WAITINGFORPREREQUISITES_H

#include "WorkState.h"

/**
 * @class WaitingForPrerequisites
 * @brief Concrete state: the work node waits for other work to be completed first.
 *
 * The node moves on to Ready once every prerequisite is Completed (the
 * mediator tells the node when one is).
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
	 * @brief Chooses the next state of a waiting node.
	 * @param workNode The node whose state is being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
