#ifndef CONCRETEWORKMEDIATOR_H
#define CONCRETEWORKMEDIATOR_H

#include "WorkMediator.h"
#include <vector>
#include <string>

class WorkNode;

/**
 * @class ConcreteWorkMediator
 * @brief Coordinates work nodes: releases nodes that wait on prerequisites.
 *
 * Holds the work nodes that take part in a workflow so they do not need to
 * know about one another.
 *
 * @par Design pattern
 * Mediator (ConcreteMediator); the colleagues are WorkNode objects.
 *
 * @see WorkMediator, WorkNode
 */
class ConcreteWorkMediator : public WorkMediator {

public:
	/** @brief The work nodes registered with this mediator. */
	std::vector<WorkNode*> colleagues;

	/**
	 * @brief Informs the registered colleagues about a change in one node.
	 * @param node The node that changed (and is the source of the message).
	 * @param msg Description of the change, for example that the node completed.
	 */
	void notifyColleagues(WorkNode* node, std::string msg);
};

#endif
