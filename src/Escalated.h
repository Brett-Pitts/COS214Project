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
	/** @brief Creates the state named "Escalated". */
	Escalated() :WorkState("Escalated") {};

	/**
	 * @brief Completes the node, and its parent too if that has nothing left to wait for.
	 * @param workNode The node being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
