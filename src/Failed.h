#ifndef FAILED_H
#define FAILED_H

#include "WorkState.h"

/**
 * @class Failed
 * @brief Concrete state: nobody could do the work and no retries are left.
 *
 * Team::assignWork() sets this state when no member can advance a work unit and
 * the retry limit has been reached. Before that, the unit is sent back to Ready
 * with WorkNode::reset(). handle() does nothing, so a failed node stays failed.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode, Team
 */
class Failed : public WorkState {
public:
	/** @brief Creates the state named "Failed". */
	Failed() :WorkState("Failed") {};

	/**
	 * @brief Does nothing, a failed node stays failed.
	 * @param workNode The node being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif // FAILED_H
