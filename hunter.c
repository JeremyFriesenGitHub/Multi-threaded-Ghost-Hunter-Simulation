#include "defs.h"

void initHunter(HunterType *hunter, const char *name, Room *startingRoom, EvidenceType equipmentType, EvidenceType *sharedEvidence) {
    strncpy(hunter->name, name, MAX_STR);
    hunter->currentRoom = startingRoom;
    hunter->equipmentType = equipmentType;
    hunter->sharedEvidence = sharedEvidence;
    hunter->fear = 0;
    hunter->boredom = 0;
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
    // Logic to move the hunter to a connected room
    hunter = hunter;
}

void collectEvidence(HunterType *hunter) {
    // Logic to collect evidence if it matches the hunter's equipment type
    hunter = hunter;
}

void reviewEvidence(HunterType *hunter) {
    // Logic to review the shared evidence
    hunter = hunter;
}

void checkHunterFearAndBoredom(HunterType *hunter) {
    if (hunter->currentRoom->ghost != NULL) {
        hunter->fear++;
        hunter->boredom = 0;
    } else {
        hunter->boredom++;
    }

    if (hunter->fear >= FEAR_MAX || hunter->boredom >= BOREDOM_MAX) {
        pthread_exit(NULL); 
    }
}

void cleanupHunter(HunterType *hunter){
    hunter = hunter;
}

