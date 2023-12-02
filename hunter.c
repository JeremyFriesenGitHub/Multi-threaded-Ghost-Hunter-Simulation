#include "defs.h"


/*
  Function: initHunter(HunterType *, const char *, Room *, EvidenceType , EvidenceList *)
  Purpose:  This function will initialize the hunter
        in     HunterType *hunter           pointer to the hunter
        in     const char *name             pointer to the name of the hunter
        in     Room *startingRoom           pointer to the starting room of hunter
        in     EvidenceType equipmentType   pointer to the equipement type for hunter
        in     EvidenceList *sharedEvidence pointer to the share evidence list for hunters

  return void
*/
void initHunter(HunterType *hunter, const char *name, Room *startingRoom, EvidenceType equipmentType, EvidenceList *sharedEvidence) {
    strncpy(hunter->name, name, MAX_STR);
    hunter->currentRoom = startingRoom;
    hunter->equipmentType = equipmentType;
    hunter->sharedEvidence = sharedEvidence;
    hunter->fear = 0;
    hunter->boredom = 0;
    addHunterToRoom(hunter, startingRoom);
    l_hunterInit( hunter->name,  hunter->equipmentType);
}

/*
  Function: initHunterNode(HunterNode *, HunterType *)
  Purpose:  This function will initialize the node for hunter
        in      HunterNode *node            pointer to the hunter node
        in      HunterType *hunter           pointer to the hunter

  return void
*/
void initHunterNode(HunterNode *node, HunterType *hunter){
    node->hunter = hunter;
    node->next = NULL;
}

/*
  Function: hunterAction(HunterType *hunter)
  Purpose:  This function will choose a random action for hunter
        in     HunterType *hunter           pointer to the hunter

  return void
*/
void hunterAction(HunterType *hunter) {
    int action = randInt(0, 4); // Assuming randInt is a function that generates a random integer
    switch(action) {
        case 0: // Move
            moveHunter(hunter);
            break;
        case 1: // Collect evidence
            collectEvidence(hunter);
            break;
        case 2: // Review evidence
            reviewEvidence(hunter);
            break;
        case 3: // Do nothing
            break;
    }

    checkHunterFearAndBoredom(hunter);
    usleep(HUNTER_WAIT);
}

/*
  Function: void moveHunter(HunterType *hunter)
  Purpose:  This function will move the hunter to a random connected room
        in     HunterType *hunter           pointer to the hunter

  return void
*/
void moveHunter(HunterType *hunter) {
    int randomRoom = randInt(0, hunter->currentRoom->connectedNum);     //choose a random room for hunter to move to

    //switch hunters room
    hunterSwitchRoom(hunter, randomRoom);

    l_hunterMove(hunter->name, hunter->currentRoom->name);

}


/*
  Function: collectEvidence(HunterType *hunter)
  Purpose:  This function will collect evidence in the room, if there is evidence that match hunter's equipment in the room add it to the share evidence list
        in     HunterType *hunter           pointer to the hunter


  return void
*/
void collectEvidence(HunterType *hunter) {
    // Logic to collect evidence if it matches the hunter's equipment type
    if(getEvidence(hunter->currentRoom->evidenceList, hunter->equipmentType) == C_TRUE) {
        //add evidence to share list
        addEvidence(hunter->sharedEvidence, hunter->equipmentType);
        l_hunterCollect(hunter->name, hunter->equipmentType, hunter->currentRoom->name);
        
    }
    
}


/*
  Function: reviewEvidence(HunterType *hunter)
  Purpose:  This function will  review the evidence in the share evidence list, if evidence is sufficeient to find the ghost, exit the thread
        in     HunterType *hunter           pointer to the hunter

  return void
*/
void reviewEvidence(HunterType *hunter) {
    // Logic to review the shared evidence
    GhostClass class = GH_UNKNOWN;

    // check if ghost is poltergeist, bamshee, bullies
    if (getEvidence(hunter->sharedEvidence, EMF) == C_TRUE){

        //Check if ghost is poltergeist or banshee
        if (getEvidence(hunter->sharedEvidence, TEMPERATURE) == C_TRUE){
            
            if (getEvidence(hunter->sharedEvidence, SOUND) == C_TRUE){
                class = BANSHEE;
            }else if (getEvidence(hunter->sharedEvidence, FINGERPRINTS) == C_TRUE){
                class = POLTERGEIST;
            }

        //Check is ghost BULLIES
        }else if (getEvidence(hunter->sharedEvidence, FINGERPRINTS) == C_TRUE){
            if (getEvidence(hunter->sharedEvidence, SOUND) == C_TRUE){
                class = BULLIES;
            }
        }

    //Check is it phantom
    }else if(getEvidence(hunter->sharedEvidence, TEMPERATURE) == C_TRUE){
        if (getEvidence(hunter->sharedEvidence, FINGERPRINTS) == C_TRUE){
            if (getEvidence(hunter->sharedEvidence, SOUND) == C_TRUE){
                class = PHANTOM;
                
            }
        }
    }

    //check is there enought evidence to identify the ghost
    if(class != GH_UNKNOWN){
        removeHunterFromRoom(hunter, hunter->currentRoom);
        l_hunterReview(hunter->name, LOG_SUFFICIENT);
        l_hunterExit(hunter->name, LOG_EVIDENCE);
        pthread_exit(NULL);
    }else{
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
    }
}


/*
  Function: checkHunterFearAndBoredom(HunterType *hunter)
  Purpose:  This function will increase or decrease hunter's fear and boredom base on their situation, 
            if the hunter's fear or boredom exceed the max fear or max boredom, the hunter thread will exit
        in     HunterType *hunter           pointer to the hunter

  return void
*/
void checkHunterFearAndBoredom(HunterType *hunter) {
    lockRoom(hunter->currentRoom);

    //check is ghost in the same room
    if (hunter->currentRoom->ghost != NULL) {
        hunter->fear++;
        hunter->boredom = 0;
    } else {
        hunter->boredom++;
    }
    unlockRoom(hunter->currentRoom);

    //check hunter fear exceed max 
    if (hunter->fear >= FEAR_MAX){
        removeHunterFromRoom(hunter, hunter->currentRoom);
        l_hunterExit(hunter->name, LOG_FEAR);
        pthread_exit(NULL);
         
    }

    //check hunter boredom exceed max or not
    if(hunter->boredom >= BOREDOM_MAX) {
        removeHunterFromRoom(hunter, hunter->currentRoom);
        l_hunterExit(hunter->name, LOG_BORED);
        pthread_exit(NULL);
    }
    
}

/*
  Function: cleanupHunters(HunterNode *hunters)
  Purpose:  This function will free all the allocated memeory for hunter
        in     HunterType *hunter           pointer to the hunter

  return void
*/
void cleanupHunters(HunterNode *hunters){
    if(hunters == NULL) return ;
    HunterNode *curr = hunters;
    HunterNode *temp = curr->next;
    free(curr);
    while (temp != NULL) {
        curr = temp; 
        temp = temp->next;
        free(curr);
    }
}

/*
  Function: hunterFunction(void *arg)
  Purpose:  This function will let hunter have action until the hunter thread exit
        in     void *arg        the data of the hunter

  return void
    NULL: The function returns NULL when the hunter thread completes its execution.
*/
void* hunterFunction(void *arg){
    HunterType *hunter = (HunterType * )arg;
    while(C_TRUE){
        hunterAction(hunter);
    }
    return NULL;
}