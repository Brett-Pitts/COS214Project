#ifndef WORKSTATE_H
#define WORKSTATE_H
#include <string>
class WorkNode;

class WorkState {
private:
	std::string name;
public:
	WorkState(std::string name) :name(name) {};
	virtual ~WorkState();
	virtual std::string getName();
	virtual void handle(WorkNode* workNode) = 0;
};

#endif
