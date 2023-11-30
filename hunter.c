#include "defs.h"

void initHunter(HunterType *hunter, const char *name, Room *startingRoom, EvidenceType equipmentType, EvidenceType *sharedEvidence) {
    strncpy(hunter->name, name, MAX_STR);
    hunter->currentRoom = startingRoom;
    hunter->equipmentType = equipmentType;
    hunter->sharedEvidence = sharedEvidence;
    hunter->fear = 0;
    hunter->boredom = 0;
    l_hunterInit( hunter->name,  hunter->equipmentType);
}


void hunterAction(HunterType *hunter) {
    int action = randInt(0, 3); // Assuming randInt is a function that generates a random integer

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
}


void moveHunter(HunterType *hunter) {
    int randomRoom = randInt(0, (hunter->currentRoom->connectedNum - 1));

    //switch hunters room
    hunterSwitchRoom(hunter, randomRoom);

    l_hunterMove(hunter->name, hunter->currentRoom->name);

}

void collectEvidence(HunterType *hunter) {
    // Logic to collect evidence if it matches the hunter's equipment type
    if(getEvidence(hunter->currentRoom->evidenceList, hunter->equipmentType) == C_TRUE) {
        //add evidence to share list
        addEvidence(hunter->sharedEvidence, hunter->equipmentType);
        l_hunterCollect(hunter->name, hunter->equipmentType, hunter->currentRoom->name);
        
    }
    
}


/*
POLTERGEIST: Leaves ghostly EMF, TEMPERATURE, and FINGERPRINTS
BANSHEE: Leaves ghostly EMF, TEMPERATURE, and SOUND
BULLIES: Leaves ghostly EMF, FINGERPRINTS, and SOUND
PHANTOM: Leaves ghostly TEMPERATURE, FINGERPRINTS, and SOUND
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
                class = BULLIES;
            }

    //Check is it phantom
    }else if(getEvidence(hunter->sharedEvidence, TEMPERATURE) == C_TRUE){
        if (getEvidence(hunter->sharedEvidence, FINGERPRINTS) == C_TRUE){
            if (getEvidence(hunter->sharedEvidence, SOUND) == C_TRUE){
                class = PHANTOM;
                
            }
        }
    }

    l_hunterReview(hunter->name, class);
    if(class != GH_UNKNOWN){
        pthread_exit(NULL);
    }
}

void checkHunterFearAndBoredom(HunterType *hunter) {
    if (hunter->currentRoom->ghost != NULL) {
        hunter->fear++;
        hunter->boredom = 0;
    } else {
        hunter->boredom++;
    }

    if (hunter->fear >= FEAR_MAX){
        l_hunterExit(hunter->name, LOG_FEAR);
        pthread_exit(NULL);
         
    }
    if(hunter->boredom >= BOREDOM_MAX) {
        l_hunterExit(hunter->name, LOG_BORED);
        pthread_exit(NULL);
    }
}

//
void cleanupHunters(HunterNode *hunters){
    if(hunters == NULL) return ;
    HunterNode *curr = hunters;
    HunterNode *temp = curr->next;
    free(curr->hunter);
    while (temp != NULL) {
        curr = temp; 
        temp = temp->next;
        free(curr->hunter); 
    }
}

