#ifndef COMPLETED_H
#define COMPLETED_H

#include "WorkState.h"

/**
 * @class Completed
 * @brief Concrete state: the work node has finished successfully.
 *
 * Reaching this state lets the parent project check whether all of its
 * children are done, and releases work that waits on this node as a
 * prerequisite.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode
 */
class Completed : public WorkState {
public:
	/** @brief Creates the state with the name "Completed". */
	Completed() :WorkState("Completed") {};

	/**
	 * @brief Handles an advance request for a completed node.
	 * @param workNode The node whose state is being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
