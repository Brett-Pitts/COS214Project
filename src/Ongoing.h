#ifndef ONGOING_H
#define ONGOING_H

#include "WorkState.h"

/**
 * @class Ongoing
 * @brief Concrete state: work has started.
 *
 * A work unit in this state can be advanced by an employee with at least Junior
 * competency (see EmployeeComponent::canAdvance()).
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode
 */
class Ongoing : public WorkState {
public:
	/** @brief Creates the state named "Ongoing". */
	Ongoing() :WorkState("Ongoing") {};

	/**
	 * @brief Moves the node to Escalated.
	 * @param workNode The node being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
