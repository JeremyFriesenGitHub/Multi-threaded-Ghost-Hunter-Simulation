#include "defs.h"



// 1. Initialize
//int main(int argc, char** argv) {
int main(){
/*
    //Check is there command line
    if (argc > 1) {

    }
*/
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;

    initHouse(&house);
    populateRooms(&house);


    HunterType hunters[NUM_HUNTERS]; // Assuming NUM_HUNTERS is 4
    GhostType ghost;
    pthread_t hunterThreads[NUM_HUNTERS], ghostThread;

    // 1.1. Ask the user to input 4 names for our hunters
    char hunterName[MAX_STR];
    for (int i = 0; i < NUM_HUNTERS; i++) {
        printf("Enter name for hunter %d: ", i + 1);
        scanf("%63s", hunterName); // MAX_STR is assumed to be 64
        initHunter(&hunters[i], hunterName, house.rooms->room, i, house.sharedEvidenceList);
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
     // 1.5. Create one thread for each hunter and one thread for the ghost
    for (int i = 0; i < NUM_HUNTERS; i++) {
        rc = pthread_create(&hunterThreads[i], NULL, hunterFunction, &hunters[i]);
        if(rc){
            printf("\nErro hunter %d", i);
            return -1;
        }
    }
    rc = pthread_create(&ghostThread, NULL, ghostFunction, &ghost);
    if(rc){
        printf("\nErro ghost");
        return -1;
    }

    // Join threads (wait for them to finish)
    for (int i = 0; i < NUM_HUNTERS; i++) {
        pthread_join(hunterThreads[i], NULL);
    }
    pthread_join(ghostThread, NULL);


    
    // 4. Finalize Results
    int fearExceeded = 0, boredomExceeded = 0;
    // 4.2. List all hunters with high fear or boredom
    printf("\n\nConclusion:");
    printf("\n==================================\n");
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
    printf("==================================\n");
    printf("..................................\n\n");


    printf("Who Won:");
    printf("\n==================================\n");

    // 4.3. Check if ghost won
    if (fearExceeded + boredomExceeded == NUM_HUNTERS) {
        printf("The ghost has won!!!!!\n");
    }else{
        printf("The hunters has won!!!!!\n");
    }
    printf("==================================\n");
    printf("..................................\n");

    // 4.4. Print collected evidence
    printf("\nThe Evidence collect by the Hunters:\n");
    for (int i = 0; i < EV_COUNT; i++) {
        if (getEvidence(house.sharedEvidenceList, i)) {
            char ev_str[MAX_STR];
            evidenceToString(i, ev_str);
            printf("      *%s\n", ev_str);
        }
    }

    // 4.5. & 4.6. Check if ghost is correctly identified
    //Check if ghost is poltergeist or banshee
    GhostClass class = GH_UNKNOWN;
    if (getEvidence(hunters[0].sharedEvidence, EMF) == C_TRUE){
        if (getEvidence(hunters[0].sharedEvidence, TEMPERATURE) == C_TRUE){
            
            if (getEvidence(hunters[0].sharedEvidence, SOUND) == C_TRUE){
                class = BANSHEE;
            }else if (getEvidence(hunters[0].sharedEvidence, FINGERPRINTS) == C_TRUE){
                class = POLTERGEIST;
            }

        //Check is ghost BULLIES
        }else if (getEvidence(hunters[0].sharedEvidence, FINGERPRINTS) == C_TRUE){
            if (getEvidence(hunters[0].sharedEvidence, SOUND) == C_TRUE){
                class = BULLIES;
            }
        }

    //Check is it phantom
    }else if(getEvidence(hunters[0].sharedEvidence, TEMPERATURE) == C_TRUE){
        if (getEvidence(hunters[0].sharedEvidence, FINGERPRINTS) == C_TRUE){
            if (getEvidence(hunters[0].sharedEvidence, SOUND) == C_TRUE){
                class = PHANTOM;
            }
        }
    }
    char ev_str[MAX_STR];
    ghostToString(class, ev_str);
    printf("\nResult:");
    printf("\nIdentified ghost: %s\n", ev_str);
    printf("Does it match the actual ghost? %s\n", class == ghost.type ? "Yes" : "No");

    // Cleanup
    cleanUpHouse(&house);


    return 0;
}