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
	 * @brief Tells the colleagues a work node changed.
	 * @param node The node that changed.
	 * @param msg Name of the node that completed.
	 */
	virtual void notifyColleagues(WorkNode* node, std::string msg) = 0;
};

#endif
