#include "ConcreteWorkMediator.h"
#include "WorkNode.h"

void ConcreteWorkMediator::notifyColleagues(WorkNode* node, std::string msg){
	for(WorkNode* colleague : colleagues){
		if(colleague != node){
			colleague->update(msg);
		}
	}
}