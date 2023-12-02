#include "defs.h"
//Testing

/*
  Function: initEvidenceList(EvidenceList *)
  Purpose:  This function will initialize the evidence list
        out    EvidenceList *list     The pointer to the list of evidence

  return void
*/
void initEvidenceList(EvidenceList *list){
    list->head = NULL;
    list->tail = NULL;
    sem_init(&list->evidenceMutex, 0, 1); // Initialize semaphore
    list->numEvidence = 0;
}

/*
  Function: getEvidence(EvidenceList *, EvidenceType )
  Purpose:  This function will check does evidence exist in the evidence list
        in     EvidenceList *list         The pointer to the list of evidence
        in     EvidenceType evidence      The pointer to the the evidence to find does it exist in the link list of evidence

  return int 
    C_FALSE - evidence not found in the list
    C_TRUE - evidence found in the list
*/
int getEvidence(EvidenceList *list, EvidenceType evidence) {

    sem_wait(&list->evidenceMutex); // Lock the semaphore

    EvidenceNode *curr = list->head;
    while(curr != NULL){
        if(curr -> evidence == evidence){
            sem_post(&list->evidenceMutex); // Unlock the semaphore
            return C_TRUE;
        }
        curr = curr->next;  // find evidenceNode contain evidence
    }
    sem_post(&list->evidenceMutex); // Unlock the semaphore
    return C_FALSE;
}

/*
  Function: addEvidence(EvidenceList *, EvidenceType )
  Purpose:  This function will add a evidence to the link list
        in     EvidenceList *list         The pointer to the list of evidence
        in     EvidenceType evidence      The pointer to the the evidence that will be added to list

  return void
*/
void addEvidence(EvidenceList *list, EvidenceType evidence){

    //lock list
    sem_wait(&list->evidenceMutex); 

    // Add evidence logic here
    EvidenceNode *newEvidence = (EvidenceNode* ) malloc(sizeof(EvidenceNode)); //create new node for evidence
    int valid = C_TRUE; //True if evidence don't exist in list

    //Fill data for new evidence
    newEvidence -> evidence = evidence;
    newEvidence -> next = NULL;

    // check is there duplivate of evidence
    EvidenceNode *node = list->head;
    while(node != NULL){
        if(node->evidence == evidence)valid = C_FALSE;
        node = node->next;
    }

    //Check is list empty
    if(list->head != NULL){
        //add to tail
        if(valid == C_TRUE){
            list->tail-> next = newEvidence;
            list->tail = newEvidence;
            list->numEvidence++;
        }else{
            //free the node because not used
            free(newEvidence);
        }
    // set new node as head
    }else{
        list->tail = newEvidence;
        list->head = newEvidence;
    }
    
    //unlock list
    sem_post(&list->evidenceMutex); 
}

/*
  Function: addEvidenceToRoom(Room *, EvidenceType )
  Purpose:  This function will add a evidence to the room
        in     Room *room                   The pointer to the room where evidences will be add to
        in     EvidenceType evidence        The pointer to the the evidence that will be added to list

  return void
*/
void addEvidenceToRoom(Room *room, EvidenceType evidence) {
    //Check is evidence valid
    if (evidence < 0 || evidence >= EV_COUNT) {
        return; // Ignore unknown or invalid evidence types
    }

    //add the evidence
    addEvidence(room->evidenceList, evidence);
}


/*
  Function: removeEvidenceFromRoom(Room *, EvidenceType )
  Purpose:  This function will remove a evidence from the room
        in     Room *room                   The pointer to the room where evidences will be remove from
        in     EvidenceType evidence        The pointer to the the evidence that will be remove from list

  return void
*/
void removeEvidenceFromRoom(Room *room, EvidenceType evidence) {
    if (evidence < 0 || evidence >= EV_COUNT) {
        return; // Ignore unknown or invalid evidence types
    }

    sem_wait(&room->evidenceList->evidenceMutex); // Lock the semaphore
    // Remove evidence logic here
    EvidenceNode *curr = room->evidenceList->head;

    //check is evidence need to be remove is first
    if(curr->evidence == evidence){
        room->evidenceList->head = curr ->next;
        free(curr);
        room->evidenceList->numEvidence--;
        sem_post(&room->evidenceList->evidenceMutex); 
        return;
    }

    //check is evidence need to be remove is last
    if(room->evidenceList->tail->evidence == evidence){
        while(curr->next ->next != NULL) curr = curr -> next; //find second last node
        free(room->evidenceList->tail);         //free tail
        room->evidenceList->tail = curr;        // set tail to second last node
        room->evidenceList->numEvidence--;
        sem_post(&room->evidenceList->evidenceMutex); 
        return;
    }

    EvidenceNode *prev;
    while(curr->evidence != evidence){
        prev = curr;        // store previous node
        curr = curr->next;  // find evidenceNode contain evidence
    }
    room->evidenceList->numEvidence--;
    prev->next = curr->next;    // remove node from list
    free(curr);                 // free node


    sem_post(&room->evidenceList->evidenceMutex); // Unlock the semaphore
}


/*
  Function: cleanupEvidences(EvidenceList *)
  Purpose:  This function will free the memory allocated for this evidence list
        in     EvidenceList *list

  return void
*/
void cleanupEvidences(EvidenceList *list){
    //check is list empty
    if(list == NULL) {
        return;
    }

    // Free evidenceList
    EvidenceNode *curr = list->head; 
    EvidenceNode *temp = NULL;
    if(curr != NULL){
        temp = curr ->next;
        free(curr);
    }
    

    //remove all evidence in list
    while (temp != NULL) {
        curr = temp; 
        temp = temp->next;
        free(curr); 
    }
    
    //destory the mutex and free the lsit
    sem_destroy(&list->evidenceMutex);
    free(list);
}
