#ifndef WORKITERATOR_H
#define WORKITERATOR_H

class WorkIterator : Iterator, Iterator {

private:
	WorkNode aggregate;
	WorkNode current;

public:
	void first();

	void next();

	bool isDone();

	WorkNode currentItem();
};

#endif
