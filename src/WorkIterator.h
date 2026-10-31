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
	virtual ~WorkIterator();

	void first();

	void next();

	bool isDone();

	WorkNode* currentItem();
};

#endif
