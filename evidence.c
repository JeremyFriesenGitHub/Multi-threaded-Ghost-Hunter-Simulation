#include "defs.h"


void addEvidenceToRoom(Room *room, EvidenceType evidence) {
    if (evidence < 0 || evidence >= EV_COUNT) {
        return; // Ignore unknown or invalid evidence types
    }

    sem_wait(&room->evidenceMutex); // Lock the semaphore

    // Add evidence logic here

    sem_post(&room->evidenceMutex); // Unlock the semaphore
}


void removeEvidenceFromRoom(Room *room, EvidenceType evidence) {
    if (evidence < 0 || evidence >= EV_COUNT) {
        return; // Ignore unknown or invalid evidence types
    }

    sem_wait(&room->evidenceMutex); // Lock the semaphore

    // Remove evidence logic here

    sem_post(&room->evidenceMutex); // Unlock the semaphore
}


//clean up list of evidences
void cleanupEvidences(EvidenceNode *head){
    if(head == NULL) return;

    // Free evidenceList
    EvidenceNode *curr = head; 
    EvidenceNode *temp = curr ->next;
    free(curr);

    //remove all evidence in list
    while (temp != NULL) {
        curr = temp; 
        temp = temp->next;
        free(curr); 
    }
}

