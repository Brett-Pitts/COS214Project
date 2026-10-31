#ifndef COMPLETED_H
#define COMPLETED_H

#include "WorkState.h"

/**
 * @class Completed
 * @brief Concrete state: the work node has finished successfully.
 *
 * This is an end state and handle() does nothing. Completion is announced
 * elsewhere: Escalated::handle() completes the parent project when its last
 * child is done, and WorkNode::advanceState() tells the mediator, which releases
 * work that waits on this node as a prerequisite.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode, Escalated
 */
class Completed : public WorkState {
public:
	/** @brief Creates the state with the name "Completed". */
	Completed() :WorkState("Completed") {};

	/**
	 * @brief Does nothing: a completed node stays completed.
	 * @param workNode The node whose state is being advanced (ignored).
	 */
	void handle(WorkNode* workNode) override;
};

#endif
