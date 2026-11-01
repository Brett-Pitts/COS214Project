#ifndef WORKITERATOR_H
#define WORKITERATOR_H

#include "Iterator.h"

/** @brief Walks a work tree. By default it only stops on ready work units. */
class WorkIterator : public Iterator {

private:
	/** The node the walk starts from. */
	WorkNode* aggregate;
	/** The node the iterator is on, or nullptr when done. */
	WorkNode* current;
	// for gui:
	bool readyOnly;

public:
	// for gui:
	WorkIterator(WorkNode* aggregate, bool readyOnly = true);
	/** @brief Destructor. */
	virtual ~WorkIterator();

	/** @brief Goes to the first match. */
	void first();

	/** @brief Goes to the next match. */
	void next();

	/** @brief True when there are no more matches. */
	bool isDone();

	/** @brief Gets the current node, or nullptr if done. */
	WorkNode* currentItem();
};

#endif
