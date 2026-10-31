#ifndef CANCELLED_H
#define CANCELLED_H

#include "WorkState.h"

/**
 * @class Cancelled
 * @brief Concrete state: the work node was cancelled and will not run.
 *
 * Cancelling a project cancels every node under it, so no ready work is left
 * behind.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode
 */
class Cancelled : public WorkState {
public:
	/** @brief Creates the state with the name "Cancelled". */
	Cancelled() :WorkState("Cancelled") {};

	/**
	 * @brief Handles an advance request for a cancelled node.
	 * @param workNode The node whose state is being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
