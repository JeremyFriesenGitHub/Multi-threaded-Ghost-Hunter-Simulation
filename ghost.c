#include "defs.h"

/*
  Function:initGhost(GhostType *, GhostClass , Room *)
  Purpose:  This function will initialize the ghost
        out     GhostType *ghost        pointer to the ghost
        in      GhostClass type         the type of ghost
        in      Room *startingRoom      the starting room of ghost

  return void
*/
void initGhost(GhostType *ghost, GhostClass type, Room *startingRoom) {
    ghost->type = type;
    ghost->currentRoom = startingRoom;
    ghost->boredomTimer = 0;

    //add the valid evidenceTypes depend on what the ghost is
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

/*
  Function: ghostAction(GhostType *ghost)
  Purpose:  This function will choose a random action for the ghost
        in      GhostType *ghost        The pointer to the ghost

  return void
*/
void ghostAction(GhostType *ghost) {
    int action = randInt(0, 3); // Assuming randInt generates a random integer between 0 and 2
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

    //check is the ghost bore
    checkGhostBoredom(ghost);
    usleep(GHOST_WAIT);
}

/*
  Function: ghostFunction(void *arg)
  Purpose:  This function will let ghost have action until ghost is bored
        in     void *arg        the data of the ghost

  return void
    NULL: The function returns NULL when the ghost thread completes its execution.
*/
void* ghostFunction(void *arg){
    GhostType *ghost = (GhostType * )arg;
    while(C_TRUE){
        ghostAction(ghost);
    }
    return NULL;
}

/*
  Function: moveGhost(GhostType *ghost)
  Purpose:  This function will move the ghost to a random connect room
        in      GhostType *ghost        The pointer to the ghost

  return void
*/
void moveGhost(GhostType *ghost) {
    lockRoom(ghost->currentRoom);

    //Move the ghost to a random connect room
    int roomIndex = randInt(0, (ghost->currentRoom->connectedNum));
    RoomNode *connectedRooms = ghost->currentRoom->connectedRooms;
    
    RoomNode *current = connectedRooms;

    //Check is there any connected room
    if (ghost->currentRoom->connectedNum == 0) {
        unlockRoom(ghost->currentRoom);
        return; // No connected rooms
    }

    // Find the room to move to
    for (int i = 0; i < roomIndex && current -> next != NULL; i++) {
        current = current->next;
    }
    
    //Remove the ghost from current room
    ghost->currentRoom->ghost = NULL;
    unlockRoom(ghost->currentRoom);


    //Add the ghost to new room
    lockRoom(current->room);
    ghost->currentRoom = current->room;
    ghost->currentRoom->ghost=ghost;
    l_ghostMove(ghost->currentRoom->name);
    unlockRoom(ghost->currentRoom);
}


/*
  Function: leaveEvidence(GhostType *)
  Purpose:  This function will leave evidence in the room the ghost is currently in
        in      GhostType *ghost        The pointer to the ghost

  return void
*/
void leaveEvidence(GhostType *ghost) {

    //lock the room
    lockRoom(ghost->currentRoom);
    if (ghost->currentRoom->evidenceList->numEvidence< MAX_EVIDENCE) { // MAX_EVIDENCE is the max number of evidence a room can hold
        int evidenceIndex = randInt(0, 3); // Randomly select one of the ghost's valid evidence types
        EvidenceType evidence = ghost->validEvidenceTypes[evidenceIndex];

        //Check is there duplicate of evidence
        //if there is duplicate evidence don't add it to evidence list to save memory
        if(getEvidence(ghost->currentRoom->evidenceList, evidence) == C_FALSE) {
            addEvidenceToRoom(ghost->currentRoom, evidence);
        }

        //log evidence
        l_ghostEvidence(evidence, ghost->currentRoom->name);
    }
    unlockRoom(ghost->currentRoom);
}

/*
  Function: checkGhostBoredom(GhostType *ghost)
  Purpose:  This function will check change the ghost boredom and will exit the thread if the ghost boredom reach BOREDOM_MAX
        in      GhostType *ghost        The pointer to the ghost

  return void
*/
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


/*
  Function: cleanUpGhost(GhostType *ghost)
  Purpose:  This function will free the memory allocated for the ghost
        in     EvidenceList *list

  return void
*/
void cleanUpGhost(GhostType *ghost) {
    free(ghost);
}

