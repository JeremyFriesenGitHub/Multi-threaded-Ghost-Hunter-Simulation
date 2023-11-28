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


