#include "defs.h"

/*
  Function: main
  Parameters: None.
  Purpose: This function serves as the entry point for the ghost hunting simulation program.
           It initializes the game environment, including the house, hunters, and ghost.
           It manages the creation of threads for hunters and the ghost, and processes their actions.
           After the simulation, it evaluates the outcome (win/lose) based on the hunters' fear and boredom levels.
           Finally, it cleans up all allocated resources before terminating.
  Return: Returns 0 for normal termination, or -1 for errors encountered during thread creation.
*/
int main(){
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;

    initHouse(&house);
    populateRooms(&house);


    HunterType hunters[NUM_HUNTERS];
    GhostType ghost;
    pthread_t hunterThreads[NUM_HUNTERS], ghostThread;


    // Collect names for each hunter and initialize them
    char hunterName[MAX_STR];
    for (int i = 0; i < NUM_HUNTERS; i++) {
        printf("Enter name for hunter %d: ", i + 1);
        scanf("%63s", hunterName); 
        initHunter(&hunters[i], hunterName, house.rooms->room, i, house.sharedEvidenceList);
    }


    // Initialize the linked list of hunters in the house
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

   // Select a random room for the ghost
    RoomNode *currentRoom = house.rooms->next; 
    int roomCount = 0;
    while (currentRoom != NULL) {
        roomCount++;
        currentRoom = currentRoom->next;
    }
    int randomRoomIndex = randInt(1, roomCount); 
    currentRoom = house.rooms->next;
    for (int i = 1; i < randomRoomIndex; i++) {
        currentRoom = currentRoom->next;
    }
    initGhost(&ghost, randomGhost(), currentRoom->room);
    
    // Create threads for hunters and handle thread creation errors
    int rc;
    for (int i = 0; i < NUM_HUNTERS; i++) {
        rc = pthread_create(&hunterThreads[i], NULL, hunterFunction, &hunters[i]);
        if(rc){
            printf("\nErro hunter %d", i);
            return -1;
        }
    }
     // Create a thread for the ghost and handle thread creation errors
    rc = pthread_create(&ghostThread, NULL, ghostFunction, &ghost);
    if(rc){
        printf("\nErro ghost");
        return -1;
    }

    // Wait for all hunter threads to complete
    for (int i = 0; i < NUM_HUNTERS; i++) {
        pthread_join(hunterThreads[i], NULL);
    }

    // Wait for the ghost thread to complete
    pthread_join(ghostThread, NULL);


    
     // Assess and display the outcome of the game based on hunters' fear and boredom levels
    
    int fearExceeded = 0, boredomExceeded = 0;
  
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

    // Determine and display the winner of the game
    printf("Who Won:");
    printf("\n==================================\n");

    
    if (fearExceeded + boredomExceeded == NUM_HUNTERS) {
        printf("The ghost has won!!!!!\n");
    }else{
        printf("The hunters have won!!!!!\n");
    }
    printf("==================================\n");
    printf("..................................\n");

    // Display the evidence collected by hunters
    printf("\nThe Evidence collected by the Hunters:\n");
    for (int i = 0; i < EV_COUNT; i++) {
        if (getEvidence(house.sharedEvidenceList, i)) {
            char ev_str[MAX_STR];
            evidenceToString(i, ev_str);
            printf("      *%s\n", ev_str);
        }
    }

    // Analyze the evidence and determine the class of the ghost
    GhostClass class = GH_UNKNOWN;
    if (getEvidence(hunters[0].sharedEvidence, EMF) == C_TRUE){
        if (getEvidence(hunters[0].sharedEvidence, TEMPERATURE) == C_TRUE){
            
            if (getEvidence(hunters[0].sharedEvidence, SOUND) == C_TRUE){
                class = BANSHEE;
            }else if (getEvidence(hunters[0].sharedEvidence, FINGERPRINTS) == C_TRUE){
                class = POLTERGEIST;
            }

        
        }else if (getEvidence(hunters[0].sharedEvidence, FINGERPRINTS) == C_TRUE){
            if (getEvidence(hunters[0].sharedEvidence, SOUND) == C_TRUE){
                class = BULLIES;
            }
        }

    
    }else if(getEvidence(hunters[0].sharedEvidence, TEMPERATURE) == C_TRUE){
        if (getEvidence(hunters[0].sharedEvidence, FINGERPRINTS) == C_TRUE){
            if (getEvidence(hunters[0].sharedEvidence, SOUND) == C_TRUE){
                class = PHANTOM;
            }
        }
    }

    // Display the result of the ghost identification
    char ev_str[MAX_STR];
    ghostToString(class, ev_str);
    printf("\nResult:");
    printf("\nIdentified ghost: %s\n", ev_str);
    printf("Does it match the actual ghost? %s\n", class == ghost.type ? "Yes" : "No");

    // Clean up and free resources associated with the house
    cleanUpHouse(&house);


    return 0;
}