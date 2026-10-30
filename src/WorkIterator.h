#ifndef WORKITERATOR_H
#define WORKITERATOR_H

#include "Iterator.h"

class WorkIterator : public Iterator {

private:
	WorkNode* aggregate;
	WorkNode* current;

public:
	WorkIterator(WorkNode* aggregate);
	virtual ~WorkIterator();

	void first();

	void next();

	bool isDone();

	WorkNode* currentItem();
};

#endif
