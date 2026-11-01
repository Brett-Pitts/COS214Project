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
	/** @brief Virtual destructor so derived aggregates clean up properly. */
	virtual ~Aggregate();

	/**
	 * @brief Hands back a new iterator over this aggregate; the caller deletes it.
	 * @return A new iterator; the caller owns it.
	 */
	virtual Iterator* createIterator() = 0;
};

#endif
