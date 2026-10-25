#include "Failed.h"
#include "Escalated.h"
#include "WorkNode.h"

void Failed::handle(WorkNode* workNode){
    if(workNode->hasRetriesLeft()){
        workNode->reset();
    }else{
        workNode->setState(new Escalated());
    }
}
