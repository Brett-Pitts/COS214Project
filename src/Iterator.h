#ifndef ITERATOR_H
#define ITERATOR_H

class WorkNode;

class Iterator {

public:
	virtual ~Iterator();

	virtual WorkNode* currentItem() = 0;

	virtual void first() = 0;

	virtual bool isDone() = 0;

	virtual void next() = 0;
};

#endif
