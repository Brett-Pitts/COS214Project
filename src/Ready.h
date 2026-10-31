#ifndef READY_H
#define READY_H

#include "WorkState.h"

/**
 * @class Ready
 * @brief Concrete state: all prerequisites are done and the work can be picked up.
 *
 * In this state an intern prepares the work. WorkIterator only stops on work
 * units in this state.
 *
 * @par Design pattern
 * State (ConcreteState); the context is WorkNode.
 *
 * @see WorkState, WorkNode, WorkIterator
 */
class Ready : public WorkState {
public:
	/** @brief Creates the state with the name "Ready". */
	Ready() :WorkState("Ready") {};

	/**
	 * @brief Chooses the next state of a ready node.
	 * @param workNode The node whose state is being advanced.
	 */
	void handle(WorkNode* workNode) override;
};

#endif
