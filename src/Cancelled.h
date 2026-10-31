#ifndef CANCELLED_H
#define CANCELLED_H

#include "WorkState.h"

/**
 * @class Cancelled
 * @brief Concrete state: the work node was cancelled and will not run.
 *
 * Manager::cancelProject() sets this state on the project it is given. This is an
 * end state and handle() does nothing. Escalated::handle() does not complete a
 * parent that is Cancelled.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode, Manager
 */
class Cancelled : public WorkState {
public:
	/** @brief Creates the state with the name "Cancelled". */
	Cancelled() :WorkState("Cancelled") {};

	/**
	 * @brief Does nothing: a cancelled node stays cancelled.
	 * @param workNode The node whose state is being advanced (ignored).
	 */
	void handle(WorkNode* workNode) override;
};

#endif
