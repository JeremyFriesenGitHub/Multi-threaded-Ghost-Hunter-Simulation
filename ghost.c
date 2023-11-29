#include "defs.h"


void initGhost(GhostType *ghost, GhostClass type, Room *startingRoom) {
    ghost->type = type;
    ghost->currentRoom = startingRoom;
    ghost->boredomTimer = 0;
    // Initialize ghost's validEvidenceTypes based on its type
    l_ghostInit(type, ghost->currentRoom->name);
}

void ghostAction(GhostType *ghost) {
    int action = randInt(0, 2); // Assuming randInt generates a random integer between 0 and 2

    switch(action) {
        case 0: // Do nothing
            break;
        case 1: // Leave evidence
            leaveEvidence(ghost);
            break;
        case 2: // Move
            if (ghost->currentRoom->hunters == NULL) { // Assuming hunters is NULL if no hunter in the room
                moveGhost(ghost);
            }
            break;
    }
}

void moveGhost(GhostType *ghost) {
    lockRoom(ghost->currentRoom);
    RoomNode *connectedRooms = ghost->currentRoom->connectedRooms;
    int numConnectedRooms = 0;

    // Count the number of connected rooms
    RoomNode *current = connectedRooms;
    while (current != NULL) {
        numConnectedRooms++;
        current = current->next;
    }

    if (numConnectedRooms == 0) return; // No connected rooms

    int roomIndex = randInt(0, numConnectedRooms - 1);
    current = connectedRooms;

    // Find the room to move to
    for (int i = 0; i < roomIndex; i++) {
        current = current->next;
    }
    ghost->currentRoom->ghost = NULL;

    unlockRooom(ghost->currentRoom);
    lockRoom(current->room);

    ghost->currentRoom = current->room;
    ghost->currentRoom->ghost=ghost;
    l_ghostMove(ghost->currentRoom->name);
    unlockRooom(ghost->currentRoom);
}



void leaveEvidence(GhostType *ghost) {
    if (ghost->currentRoom->evidenceList->numEvidence< MAX_EVIDENCE) { // MAX_EVIDENCE is the max number of evidence a room can hold
        int evidenceIndex = randInt(0, 2); // Randomly select one of the ghost's valid evidence types
        EvidenceType evidence = ghost->validEvidenceTypes[evidenceIndex];

        addEvidenceToRoom(ghost->currentRoom, evidence);
        l_ghostEvidence(evidence, ghost->currentRoom->name);
    }
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


//need to change
void cleanUpGhost(GhostType *ghost) {
    // Implement if there are resources to be cleaned up
    ghost = ghost;
}
