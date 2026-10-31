#ifndef FAILED_H
#define FAILED_H

#include "WorkState.h"

/**
 * @class Failed
 * @brief Concrete state: nobody could do the work.
 *
 * Failed work goes back to Ready until the retry limit is reached; after that
 * it stays failed.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode
 */
class Failed : public WorkState {
public:
	/** @brief Creates the state with the name "Failed". */
	Failed() :WorkState("Failed") {};

	/**
	 * @brief Handles an advance request for a failed node (retry or remain failed).
	 * @param workNode The node whose state is being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif // FAILED_H
