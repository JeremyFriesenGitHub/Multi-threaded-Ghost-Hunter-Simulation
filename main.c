#include "defs.h"



    // 1. Initialize
//int main(int argc, char** argv) {
int main(){
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;

    initHouse(&house);
    populateRooms(&house);

    printf("Room in house: %s", house.rooms->room->name);

    HunterType hunters[NUM_HUNTERS]; // Assuming NUM_HUNTERS is 4
    GhostType ghost;
    pthread_t hunterThreads[NUM_HUNTERS], ghostThread;

    // 1.1. Ask the user to input 4 names for our hunters
    char hunterName[MAX_STR];
    for (int i = 0; i < NUM_HUNTERS; i++) {
        printf("Enter name for hunter %d: ", i + 1);
        scanf("%63s", hunterName); // MAX_STR is assumed to be 64
        initHunter(&hunters[i], hunterName, house.rooms->room, randInt(0, EV_COUNT), house.sharedEvidenceList);
    }


    // Add hunter link list to house
    HunterNode *curr = malloc(sizeof(HunterNode));
    initHunterNode(curr, &hunters[0]);
    house.hunters = curr;
    house.numHunters++;
    for (int i = 0; i < NUM_HUNTERS; i++) {
        HunterNode *node = malloc(sizeof(HunterNode));
        initHunterNode(node, &hunters[i]);
        curr->next = node;
        curr = node;
        house.numHunters++;

    }

    // 1.2. Populate the house with the provided rooms
    // This has been done above with populateRooms(&house);

    // 1.3. Place the 4 hunters in the Van room
    // This is already done in the loop where hunters are initialized

    // 1.4. Place the ghost in a random room, not the Van
    RoomNode *currentRoom = house.rooms->next; // Skip the Van room
    int roomCount = 0;
    while (currentRoom != NULL) {
        roomCount++;
        currentRoom = currentRoom->next;
    }
    int randomRoomIndex = randInt(1, roomCount); // Generate a random index
    currentRoom = house.rooms->next; // Reset to first room (not Van)
    for (int i = 1; i < randomRoomIndex; i++) {
        currentRoom = currentRoom->next;
    }
    initGhost(&ghost, randomGhost(), currentRoom->room);
    int rc;
    printf("threading going to be made");
     // 1.5. Create one thread for each hunter and one thread for the ghost
    for (int i = 0; i < NUM_HUNTERS; i++) {
        rc = pthread_create(&hunterThreads[i], NULL, hunterFunction, &hunters[i]);
        if(rc){
            printf("\nhunter %d", i);
            return -1;
        }
    }
    rc = pthread_create(&ghostThread, NULL, ghostFunction, &ghost);
    if(rc){
        printf("\nghost");
        return -1;
    }

    // Join threads (wait for them to finish)
    for (int i = 0; i < NUM_HUNTERS; i++) {
        pthread_join(hunterThreads[i], NULL);
    }
    pthread_join(ghostThread, NULL);
    // 4. Finalize Results
    int fearExceeded = 0, boredomExceeded = 0;
    EvidenceType collectedEvidence[EV_COUNT] = {0}; // Assuming EV_COUNT is the number of evidence types
    collectedEvidence[0] = 1;
    printf("%d", collectedEvidence[0]);
    // 4.2. List all hunters with high fear or boredom
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (hunters[i].fear >= FEAR_MAX) {
            printf("Hunter %s exceeded max fear.\n", hunters[i].name);
            fearExceeded++;
        }
        if (hunters[i].boredom >= BOREDOM_MAX) {
            printf("Hunter %s exceeded max boredom.\n", hunters[i].name);
            boredomExceeded++;
        }
    }

    // 4.3. Check if ghost won
    if (fearExceeded + boredomExceeded == NUM_HUNTERS) {
        printf("The ghost has won.\n");
    }

    // 4.4. Print collected evidence
    printf("Collected Evidence:\n");
    for (int i = 0; i < EV_COUNT; i++) {
        if (getEvidence(house.sharedEvidenceList, i)) {
            char ev_str[MAX_STR];
            evidenceToString(i, ev_str);
            printf("%s\n", ev_str);
            collectedEvidence[i] = 1;
        }
    }

    // 4.5. & 4.6. Check if ghost is correctly identified
    /*GhostClass identifiedGhost = identifyGhost(collectedEvidence);
    if (identifiedGhost != GH_UNKNOWN) {
        char ev_str[MAX_STR];
        ghostToString(identifiedGhost, ev_str);
        printf("Identified ghost: %s\n", ev_str);
        printf("Does it match the actual ghost? %s\n", identifiedGhost == ghost.type ? "Yes" : "No");
    }*/

    // Cleanup
    cleanUpHouse(&house);
    cleanUpGhost(&ghost);


    return 0;
}

void* hunterFunction(void *arg){
    HunterType *hunter = (HunterType * )arg;
    printf("\nHunter room: %s", hunter->name);
    /*while(C_TRUE){
        hunterAction(hunter);
    }*/
    return NULL;
}

void* ghostFunction(void *arg){
    GhostType *ghost = (GhostType * )arg;
    printf("\nGhost room: %s", ghost->currentRoom->name);
    /*while(C_TRUE){
        hunterAction(hunter);
    }*/
    /*while(C_TRUE){
        ghostAction(ghost);
    }*/
    return NULL;
}