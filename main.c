#include "defs.h"


int main(int argc, char** argv) {

    //Check is there command line
    if (argc > 1) {

    }

    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;
    HunterType hunter1, hunter2, hunter3, hunter4;
    GhostType ghost;
    EvidenceList shareEvidence;

    initHouse(&house);
    populateRooms(&house);

    initEvidenceList(&shareEvidence);


    initHunter(hunter1, "one", house.rooms->room, 1, &shareEvidence);
    initHunter(hunter2, "two", house.rooms->room, 1, &shareEvidence);
    initHunter(hunter3, "three", house.rooms->room, 1, &shareEvidence);
    initHunter(hunter4, "four", house.rooms->room, 1, &shareEvidence);

    initGhost(&ghost, 1, house.rooms->next->next->room);

/*
    pthread_create(&monkey1, NULL, monkeySort, &house);
    pthread_create(&monkey2, NULL, monkeySort, &house);
    pthread_create(&monkey1, NULL, monkeySort, &house);
    pthread_create(&monkey2, NULL, monkeySort, &house);
    pthread_create(&monkey2, NULL, monkeySort, &house);
    

    // Wait for the threads to finish and join them
    pthread_join(monkey1, NULL);
    pthread_join(monkey2, NULL);
    pthread_join(monkey1, NULL);
    pthread_join(monkey2, NULL);
    pthread_join(monkey2, NULL);
*/

    return 0;
}

