#include "Escalated.h"
#include "Completed.h"
#include "WorkNode.h"

void Escalated::handle(WorkNode* workNode){
    if(workNode->allChildrenCompleted()){
        WorkNode* parent = workNode->getParent();
        workNode->setState(new Completed());
        if(parent != nullptr && parent->getStateName() != "Cancelled" && parent->allChildrenCompleted()){
            parent->setState(new Escalated());
            parent->advanceState();
        }
    }
}
