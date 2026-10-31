#ifndef ESCALATED_H
#define ESCALATED_H

#include "WorkState.h"

/**
 * @class Escalated
 * @brief Concrete state: the work needs a senior employee to finish it.
 *
 * Escalated does not mean the work failed. It is also entered when QA finds
 * bugs or an inspection fails, so the unit is redone at senior level.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode
 */
class Escalated : public WorkState {
public:
	/** @brief Creates the state with the name "Escalated". */
	Escalated() :WorkState("Escalated") {};

	/**
	 * @brief Chooses the next state of an escalated node.
	 * @param workNode The node whose state is being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
