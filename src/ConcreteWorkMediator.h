#ifndef CONCRETEWORKMEDIATOR_H
#define CONCRETEWORKMEDIATOR_H

#include "WorkMediator.h"
#include <vector>
#include <string>

class WorkNode;

/**
 * @class ConcreteWorkMediator
 * @brief Passes the completion of one work node on to all the others, which releases nodes that wait on it.
 *
 * Holds the work nodes that take part in a workflow so they do not need to know
 * about one another. Nodes are not registered automatically: the caller has to put
 * them in colleagues and give each of them this mediator with
 * WorkNode::setMediator().
 *
 * @par Design pattern
 * Mediator (ConcreteMediator); the colleagues are WorkNode objects.
 *
 * @see WorkMediator, WorkNode
 */
class ConcreteWorkMediator : public WorkMediator {

public:
	/**
	 * @brief The work nodes registered with this mediator. They are not owned, and are added by the caller directly.
	 */
	std::vector<WorkNode*> colleagues;

	/**
	 * @brief Calls WorkNode::update() with the message on every registered colleague except the sender.
	 *
	 * WorkNode::update() treats the message as the name of a completed node and
	 * removes it from the node's prerequisites, which can move a waiting node to Ready.
	 *
	 * @param node The node that changed. It is not updated.
	 * @param msg The name of the node that completed.
	 */
	void notifyColleagues(WorkNode* node, std::string msg);
};

#endif
