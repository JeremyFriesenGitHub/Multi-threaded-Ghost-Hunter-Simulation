#include "defs.h"

/*
  Function: initHunter
  Parameters: 
    HunterType *hunter - Pointer to the hunter.
    const char *name - Name of the hunter.
    Room *startingRoom - Starting room of the hunter.
    EvidenceType equipmentType - Equipment type of the hunter.
    EvidenceList *sharedEvidence - Shared evidence list among hunters.
  Purpose: Initializes a hunter with specified name, starting room, equipment type, and shared evidence list.
  Return: void
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
  Function: initHunterNode
  Parameters:
    HunterNode *node - Pointer to the hunter node.
    HunterType *hunter - Pointer to the hunter.
  Purpose: Initializes a hunter node with the given hunter.
  Return: void
*/
void initHunterNode(HunterNode *node, HunterType *hunter){
    node->hunter = hunter;
    node->next = NULL;
}

/*
  Function: hunterAction
  Parameters:
    HunterType *hunter - Pointer to the hunter.
  Purpose: Chooses and executes a random action for the hunter.
  Return: void
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
  Function: moveHunter
  Parameters:
    HunterType *hunter - Pointer to the hunter.
  Purpose: Moves the hunter to a random connected room.
  Return: void
*/
void moveHunter(HunterType *hunter) {
    int randomRoom = randInt(0, hunter->currentRoom->connectedNum);     //choose a random room for hunter to move to

    //switch hunters room
    hunterSwitchRoom(hunter, randomRoom);

    l_hunterMove(hunter->name, hunter->currentRoom->name);

}


/*
  Function: collectEvidence
  Parameters:
    HunterType *hunter - Pointer to the hunter.
  Purpose: Collects evidence in the room if it matches the hunter's equipment type.
  Return: void
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
  Function: reviewEvidence
  Parameters:
    HunterType *hunter - Pointer to the hunter.
  Purpose: Reviews the shared evidence list and determines if sufficient evidence has been collected to identify the ghost.
  Return: void
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
  Function: checkHunterFearAndBoredom
  Parameters:
    HunterType *hunter - Pointer to the hunter.
  Purpose: Modifies the hunter's fear and boredom levels based on the situation, and exits the thread if certain thresholds are exceeded.
  Return: void
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
  Function: cleanupHunters
  Parameters:
    HunterNode *hunters - Pointer to the head of the hunters list.
  Purpose: Frees all allocated memory associated with hunters.
  Return: void
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
  Function: hunterFunction
  Parameters:
    void *arg - Pointer to the hunter data.
  Purpose: Executes hunter actions repeatedly in a thread until the thread exits.
  Return: NULL - Indicates the thread's completion.
*/
void* hunterFunction(void *arg){
    HunterType *hunter = (HunterType * )arg;
    while(C_TRUE){
        hunterAction(hunter);
    }
    return NULL;
}