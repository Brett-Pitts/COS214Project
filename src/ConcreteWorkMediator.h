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
	/** @brief The registered work nodes, not owned. */
	std::vector<WorkNode*> colleagues;

	/**
	 * @brief Tells every registered node except the sender that msg has completed.
	 * @param node The node that changed.
	 * @param msg Name of the completed node.
	 */
	void notifyColleagues(WorkNode* node, std::string msg);
};

#endif
