#ifndef READY_H
#define READY_H

#include "WorkState.h"

class Ready : public WorkState {
public:
	Ready(std::string name) :WorkState(name) {};

	void handle(WorkNode* workNode) override;
};

#endif
