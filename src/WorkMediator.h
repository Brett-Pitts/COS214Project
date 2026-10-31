#ifndef WORKMEDIATOR_H
#define WORKMEDIATOR_H
#include <string>

class WorkNode;

/**
 * @class WorkMediator
 * @brief Abstract mediator that lets work nodes communicate without knowing each other.
 *
 * @par Design pattern
 * Mediator (Mediator); the colleagues are WorkNode objects.
 *
 * @see ConcreteWorkMediator, WorkNode
 */
class WorkMediator {


public:
	/**
	 * @brief Informs the colleagues about a change in one work node.
	 * @param node The node that changed.
	 * @param msg Description of the change. WorkNode::update() reads it as the name of a node that completed.
	 */
	virtual void notifyColleagues(WorkNode* node, std::string msg) = 0;
};

#endif
