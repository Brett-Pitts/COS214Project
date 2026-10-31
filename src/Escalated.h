#ifndef ESCALATED_H
#define ESCALATED_H

#include "WorkState.h"

/**
 * @class Escalated
 * @brief Concrete state: the last step before completion, which needs a senior employee.
 *
 * Despite the name, being Escalated does not mean that something went wrong:
 * every work unit passes through this state on its way to Completed. A work unit
 * here can only be advanced by an employee with Senior competency (see
 * EmployeeComponent::canAdvance()).
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
	 * @brief Completes the node, and then its parent if that has nothing left to wait for.
	 *
	 * If the node has unfinished children (only possible for a Project), nothing
	 * happens. Otherwise the node is set to Completed. Then, if it has a parent that
	 * is not Cancelled and all of the parent's children are now completed, the parent
	 * is set to Escalated and advanced as well, so completion moves up the tree one
	 * level at a time.
	 *
	 * @param workNode The node whose state is being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
