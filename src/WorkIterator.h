#ifndef WORKITERATOR_H
#define WORKITERATOR_H

#include "Iterator.h"

/**
 * @class WorkIterator
 * @brief Walks a work tree depth first and stops on work units that are Ready.
 *
 * Parents are visited before their children and children in the order they were
 * added. Only WorkUnit objects in the Ready state are returned; projects and
 * units in any other state are skipped. The iterator only keeps the starting
 * node and the current node, and moves with getChild() and getParent().
 *
 * A new iterator is not on any item yet: call first() before currentItem().
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
	 *
	 * The iterator is not on any item yet, so isDone() is true until first() is called.
	 *
	 * @param aggregate The node to start from (usually a Project).
	 */
	WorkIterator(WorkNode* aggregate);

	/** @brief Virtual destructor. */
	virtual ~WorkIterator();

	/**
	 * @brief Moves to the first Ready work unit in the tree.
	 *
	 * The starting node itself is checked first. If there is no Ready work unit,
	 * isDone() becomes true.
	 */
	void first();

	/**
	 * @brief Moves to the next Ready work unit in depth-first order.
	 *
	 * Does nothing if the iteration is already done (or first() was not called).
	 */
	void next();

	/**
	 * @brief Checks whether the iteration has finished.
	 * @return true if there is no current item: there are no more Ready work units, or first() has not been called yet.
	 */
	bool isDone();

	/**
	 * @brief Gets the node the iterator is on.
	 * @return The current node, or nullptr if the iterator is done.
	 */
	WorkNode* currentItem();
};

#endif
