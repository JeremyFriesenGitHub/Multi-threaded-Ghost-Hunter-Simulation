#include "defs.h"


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
    HunterType hunter1, hunter2, hunter3, hunter4;
    pthread_t h1,h2,h3,h4, g;
    GhostType ghost;
    EvidenceList shareEvidence;

    initHouse(&house);
    populateRooms(&house);

    initEvidenceList(&shareEvidence);


    initHunter(&hunter1, "one", house.rooms->room, 1, &shareEvidence);
    initHunter(&hunter2, "two", house.rooms->room, 2, &shareEvidence);
    initHunter(&hunter3, "three", house.rooms->room, 3, &shareEvidence);
    initHunter(&hunter4, "four", house.rooms->room, 0, &shareEvidence);

    initGhost(&ghost, 1, house.rooms->next->next->room);


    pthread_create(&h1, NULL, somefunction, &house);
    pthread_create(&h2, NULL, somefunction, &house);
    pthread_create(&h3, NULL, somefunction, &house);
    pthread_create(&h4, NULL, somefunction, &house);
    pthread_create(&g, NULL, somefunction, &house);
    

    // Wait for the threads to finish and join them
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);
    pthread_join(g, NULL);


    return 0;
}

