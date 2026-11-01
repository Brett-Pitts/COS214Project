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
 * Iterator (Iterator). WorkIterator is the concrete iterator. TeamIterator uses
 * the same first, next, isDone and currentItem operations but does not derive
 * from this class, because it returns Team objects rather than work nodes.
 *
 * @see Aggregate, WorkIterator, TeamIterator
 */
class Iterator {

public:
	/** @brief Virtual destructor. */
	virtual ~Iterator();

	/**
	 * @brief Gets the current node, or nullptr when done.
	 * @return The current item, or nullptr when done.
	 */
	virtual WorkNode* currentItem() = 0;

	/** @brief Jumps to the first matching item. */
	virtual void first() = 0;

	/**
	 * @brief True when there is nothing left.
	 * @return True when finished.
	 */
	virtual bool isDone() = 0;

	/** @brief Moves to the next matching item. */
	virtual void next() = 0;
};

#endif
