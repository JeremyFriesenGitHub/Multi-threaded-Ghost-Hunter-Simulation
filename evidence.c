#include "defs.h"


void initEvidenceList(EvidenceList *list){
    list->head = NULL;
    list->tail = NULL;
    sem_init(&list->evidenceMutex, 0, 1); // Initialize semaphore
}

int getEvidence(Room *room, EvidenceType evidence) {

    sem_wait(&room->evidenceList->evidenceMutex); // Lock the semaphore

    EvidenceNode *curr = room->evidenceList->head;
    while(curr != NULL){
        if(curr -> evidence == evidence){
            return C_TRUE;
        }
        curr = curr->next;  // find evidenceNode contain evidence
    }

    return C_FALSE;

    sem_post(&room->evidenceList->evidenceMutex); // Unlock the semaphore
}

void addEvidence(EvidenceList *list, EvidenceType evidence){

    sem_wait(&list->evidenceMutex); 

    // Add evidence logic here
    EvidenceNode *newEvidence = (EvidenceNode* ) malloc(sizeof(EvidenceNode)); //create new node for evidence

    //Fill data for new evidence
    newEvidence -> evidence = evidence;
    newEvidence -> next = NULL;

    // add to tail and set tail to new evidence
    list->tail-> next = newEvidence;
    list->tail = newEvidence;
    
    sem_post(&list->evidenceMutex); 
}

void addEvidenceToRoom(Room *room, EvidenceType evidence) {
    if (evidence < 0 || evidence >= EV_COUNT) {
        return; // Ignore unknown or invalid evidence types
    }

    sem_wait(&room->evidenceList->evidenceMutex); // Lock the semaphore

    // Add evidence logic here
    room->numEvidence++;

    sem_post(&room->evidenceList->evidenceMutex); // Unlock the semaphore

    addEvidence(room->evidenceList, evidence);
}


void removeEvidenceFromRoom(Room *room, EvidenceType evidence) {
    if (evidence < 0 || evidence >= EV_COUNT) {
        return; // Ignore unknown or invalid evidence types
    }

    sem_wait(&room->roomLock);

    room->numEvidence--;

    sem_post(&room->roomLock);


    sem_wait(&room->evidenceList->evidenceMutex); // Lock the semaphore
    room->numEvidence--;
    // Remove evidence logic here
    EvidenceNode *curr = room->evidenceList->head;
    //check is evidence need to be remove is first
    if(curr->evidence == evidence){
        room->evidenceList->head = curr ->next;
        free(curr);
        return;
    }

    //check is evidence need to be remove is last
    if(room->evidenceList->tail->evidence == evidence){
        while(curr->next ->next != NULL) curr = curr -> next; //find second last node
        free(room->evidenceList->tail);         //free tail
        room->evidenceList->tail = curr;        // set tail to second last node
        return;
    }

    EvidenceNode *prev;
    while(curr->evidence != evidence){
        prev = curr;        // store previous node
        curr = curr->next;  // find evidenceNode contain evidence
    }
    
    prev->next = curr->next;    // remove node from list
    free(curr);                 // free node


    sem_post(&room->evidenceList->evidenceMutex); // Unlock the semaphore
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
