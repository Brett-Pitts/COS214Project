#ifndef WORKITERATOR_H
#define WORKITERATOR_H

#include "Iterator.h"

class WorkIterator : public Iterator {

private:
	WorkNode* aggregate;
	WorkNode* current;
	// for gui:
	bool readyOnly;

public:
	// for gui:
	WorkIterator(WorkNode* aggregate, bool readyOnly = true);

	/** @brief Virtual destructor. */
	virtual ~WorkIterator();

	/** @brief Goes to the first matching node. */
	void first();

	/** @brief Moves to the next matching node. */
	void next();

	/**
	 * @brief True when there are no more nodes.
	 * @return True when finished.
	 */
	bool isDone();

	/**
	 * @brief Gets the current node, or nullptr when done.
	 * @return The current item, or nullptr when done.
	 */
	WorkNode* currentItem();
};

#endif
