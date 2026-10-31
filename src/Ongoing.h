#ifndef ONGOING_H
#define ONGOING_H

#include "WorkState.h"

/**
 * @class Ongoing
 * @brief Concrete state: work has started and a junior performs it.
 *
 * A project is Ongoing until all of its children are completed.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode
 */
class Ongoing : public WorkState {
public:
	/** @brief Creates the state with the name "Ongoing". */
	Ongoing() :WorkState("Ongoing") {};

	/**
	 * @brief Chooses the next state of an ongoing node.
	 * @param workNode The node whose state is being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
