#include "WaitingForPrerequisites.h"
#include "Ready.h"
#include "WorkNode.h"

void WaitingForPrerequisites::handle(WorkNode* workNode){
    if(workNode->prerequisitesCompleted()){
        workNode->setState(new Ready());
    }
}
