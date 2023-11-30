#include "defs.h"


void initGhost(GhostType *ghost, GhostClass type, Room *startingRoom) {
    ghost->type = type;
    ghost->currentRoom = startingRoom;
    ghost->boredomTimer = 0;
    if(type == 0){
        ghost->validEvidenceTypes[0] = 0;
        ghost->validEvidenceTypes[1] = 1;
        ghost->validEvidenceTypes[2] = 2;
    }
    if(type == 1){
        ghost->validEvidenceTypes[0] = 0;
        ghost->validEvidenceTypes[1] = 1;
        ghost->validEvidenceTypes[2] = 3;
    }
    if(type == 2){
        ghost->validEvidenceTypes[0] = 0;
        ghost->validEvidenceTypes[1] = 2;
        ghost->validEvidenceTypes[2] = 3;
    }
    if(type == 3){
        ghost->validEvidenceTypes[0] = 1;
        ghost->validEvidenceTypes[1] = 2;
        ghost->validEvidenceTypes[2] = 3;
    }

    // Initialize ghost's validEvidenceTypes based on its type
    l_ghostInit(type, ghost->currentRoom->name);
}

void ghostAction(GhostType *ghost) {
    int action = randInt(0, 3); // Assuming randInt generates a random integer between 0 and 2
    printf("\nghost action:%d",action);
    switch(action) {
        case 0: // Do nothing
            break;
        case 1: // Leave evidence
            leaveEvidence(ghost);
            break;
        case 2: // Move
            if (ghost->currentRoom->hunters == NULL) { // Assuming hunters is NULL if no hunter in the room
                moveGhost(ghost);
            }else{
                printf("I see a hunter%s", ghost->currentRoom->hunters->hunter->currentRoom->name);
            }
            break;
    }
    checkGhostBoredom(ghost);
    usleep(GHOST_WAIT);
}

void moveGhost(GhostType *ghost) {
    lockRoom(ghost->currentRoom);
    int roomIndex = randInt(0, (ghost->currentRoom->connectedNum));
    RoomNode *connectedRooms = ghost->currentRoom->connectedRooms;
    // Count the number of connected rooms
    RoomNode *current = connectedRooms;

    if (ghost->currentRoom->connectedNum == 0) {
        unlockRoom(ghost->currentRoom);
        return; // No connected rooms
    }

    // Find the room to move to
    for (int i = 0; i < roomIndex && current -> next != NULL; i++) {
        current = current->next;
    }
   
    ghost->currentRoom->ghost = NULL;
    unlockRoom(ghost->currentRoom);


    lockRoom(current->room);

    ghost->currentRoom = current->room;
    ghost->currentRoom->ghost=ghost;
    l_ghostMove(ghost->currentRoom->name);
    unlockRoom(ghost->currentRoom);
}



void leaveEvidence(GhostType *ghost) {
    lockRoom(ghost->currentRoom);
    if (ghost->currentRoom->evidenceList->numEvidence< MAX_EVIDENCE) { // MAX_EVIDENCE is the max number of evidence a room can hold
        int evidenceIndex = randInt(0, 3); // Randomly select one of the ghost's valid evidence types
        EvidenceType evidence = ghost->validEvidenceTypes[evidenceIndex];

        addEvidenceToRoom(ghost->currentRoom, evidence);
        l_ghostEvidence(evidence, ghost->currentRoom->name);
    }
    unlockRoom(ghost->currentRoom);
}

void checkGhostBoredom(GhostType *ghost) {
    if (ghost->currentRoom->hunters == NULL) {
        ghost->boredomTimer++;
    } else {
        ghost->boredomTimer = 0;
    }

    if (ghost->boredomTimer >= BOREDOM_MAX) {
        l_ghostExit(LOG_BORED);
        pthread_exit(NULL); 
    }
}


//done
void cleanUpGhost(GhostType *ghost) {
    free(ghost);/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> a8a2423 (Updated Main Function WorkFlow)*/
}


void *ghostThreadFunction(void *arg) {
    GhostType *ghost = (GhostType *)arg;

    while (1) {
        lockRoom(ghost->currentRoom);

        // 2.1. Ghost in the same room as a hunter
        if (ghost->currentRoom->hunters != NULL) {
            ghost->boredomTimer = 0; // Reset boredom timer
            int action = randInt(0, 1); // Randomly choose to leave evidence or do nothing

            if (action == 0) { // Leave evidence
                leaveEvidence(ghost);
            }
            // Else, do nothing
        }
        // 2.2. Ghost not in the same room as a hunter
        else {
            ghost->boredomTimer++; // Increase boredom counter
            int action = randInt(0, 2); // Randomly choose action

            if (action == 0) { // Move to an adjacent room
                moveGhost(ghost);
            } 
            else if (action == 1) { // Leave evidence
                leaveEvidence(ghost);
            }
            // Else, do nothing
        }

        unlockRoom(ghost->currentRoom);

        // 2.5. Check boredom level
        if (ghost->boredomTimer >= BOREDOM_MAX) {
            break; // Exit the thread
        }

        // Optional: sleep for a short duration
        usleep(GHOST_WAIT); // GHOST_WAIT is a predefined constant for wait time
    }

    return NULL;

}
