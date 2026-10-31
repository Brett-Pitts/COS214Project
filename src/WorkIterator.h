#ifndef WORKITERATOR_H
#define WORKITERATOR_H

#include "Iterator.h"

/**
 * @class WorkIterator
 * @brief Walks a work tree depth first and stops on work units that are Ready.
 *
 * It only keeps the starting node and the current node, and moves with
 * getChild() and getParent().
 *
 * @par Design pattern
 * Iterator (ConcreteIterator); the aggregate is WorkNode.
 *
 * @see Iterator, WorkNode
 */
class WorkIterator : public Iterator {

private:
	WorkNode* aggregate; ///< The node the iteration started from. Not owned.
	WorkNode* current;   ///< The node the iterator is on. Not owned.

public:
	/**
	 * @brief Creates an iterator for a work tree.
	 * @param aggregate The node to start from (usually a Project).
	 */
	WorkIterator(WorkNode* aggregate);

	/** @brief Virtual destructor. */
	virtual ~WorkIterator();

	/** @brief Moves to the first Ready work unit in the tree. */
	void first();

	/** @brief Moves to the next Ready work unit in depth-first order. */
	void next();

	/**
	 * @brief Checks whether the iteration has finished.
	 * @return true if there are no more Ready work units.
	 */
	bool isDone();

	/**
	 * @brief Gets the node the iterator is on.
	 * @return The current node, or nullptr if the iterator is done.
	 */
	WorkNode* currentItem();
};

#endif
