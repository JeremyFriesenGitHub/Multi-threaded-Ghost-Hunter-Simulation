#include "defs.h"


void initEvidenceList(EvidenceList *list){
    list->head = NULL;
    list->tail = NULL;
}

void addEvidenceToRoom(Room *room, EvidenceType evidence) {
    if (evidence < 0 || evidence >= EV_COUNT) {
        return; // Ignore unknown or invalid evidence types
    }

    sem_wait(&room->evidenceMutex); // Lock the semaphore

    // Add evidence logic here
    EvidenceNode *newEvidence = (EvidenceNode* ) malloc(sizeof(EvidenceNode)); //create new node for evidence

    //Fill data for new evidence
    newEvidence -> evidence = evidence;
    newEvidence -> next = NULL;

    // add to tail and set tail to new evidence
    room->evidenceList->tail-> next = newEvidence;
    room->evidenceList->tail = newEvidence;
    room->numEvidence++;

    sem_post(&room->evidenceMutex); // Unlock the semaphore
}


void removeEvidenceFromRoom(Room *room, EvidenceType evidence) {
    if (evidence < 0 || evidence >= EV_COUNT) {
        return; // Ignore unknown or invalid evidence types
    }

    sem_wait(&room->evidenceMutex); // Lock the semaphore
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

