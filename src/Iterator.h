#ifndef ITERATOR_H
#define ITERATOR_H

class WorkNode;

/**
 * @class Iterator
 * @brief Abstract iterator over work nodes.
 *
 * Lets clients walk a work tree (for example to find the next ready work
 * unit) without knowing how the tree is stored.
 *
 * @par Design pattern
 * Iterator (Iterator). WorkIterator and TeamIterator are concrete iterators.
 *
 * @see Aggregate
 */
class Iterator {

public:
	/** @brief Virtual destructor so derived iterators are destroyed correctly. */
	virtual ~Iterator();

	/**
	 * @brief Gets the node the iterator is currently on.
	 * @return The current node, or nullptr if the iterator is done.
	 */
	virtual WorkNode* currentItem() = 0;

	/** @brief Moves to the first matching item. */
	virtual void first() = 0;

	/**
	 * @brief Checks whether the iteration has finished.
	 * @return true if there are no more items.
	 */
	virtual bool isDone() = 0;

	/** @brief Moves to the next matching item. */
	virtual void next() = 0;
};

#endif
