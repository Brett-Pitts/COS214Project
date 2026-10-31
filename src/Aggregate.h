#ifndef AGGREGATE_H
#define AGGREGATE_H

class Iterator;

/**
 * @file Aggregate.h
 * @brief Declares the Aggregate interface of the Iterator pattern.
 */

/**
 * @class Aggregate
 * @brief Abstract collection that can hand out an Iterator over its contents.
 *
 * @par Design pattern
 * Iterator (Aggregate). WorkNode and its subclasses implement this interface
 * so that clients can find ready work units without knowing how the work tree
 * is stored.
 *
 * @see Iterator
 */
class Aggregate {

public:
	/** @brief Virtual destructor so derived aggregates are destroyed correctly. */
	virtual ~Aggregate();

	/**
	 * @brief Creates an iterator over this aggregate.
	 * @return A newly allocated Iterator. The caller owns it and must delete it.
	 */
	virtual Iterator* createIterator() = 0;
};

#endif
