#ifndef WORKITERATOR_H
#define WORKITERATOR_H

#include "Iterator.h"
class WorkNode;

class WorkIterator : public Iterator {

private:
	WorkNode* aggregate;
	WorkNode* current;

public:
	void first();

	void next();

	bool isDone();

	WorkNode* currentItem();
};

#endif
