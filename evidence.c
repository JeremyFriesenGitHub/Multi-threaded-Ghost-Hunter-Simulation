#include "defs.h"

/*
  Function: initEvidenceList
  Parameters:
    EvidenceList *list - Pointer to the list of evidence to be initialized.
  Purpose: Initializes the evidence list with default values and prepares it for use.
  Return: void
*/
void initEvidenceList(EvidenceList *list){
    list->head = NULL;
    list->tail = NULL;
    sem_init(&list->evidenceMutex, 0, 1); // Initialize semaphore
    list->numEvidence = 0;
}

/*
  Function: getEvidence
  Parameters:
    EvidenceList *list - Pointer to the list of evidence.
    EvidenceType evidence - The evidence type to check in the list.
  Purpose: Checks whether a specific type of evidence exists in the evidence list.
  Return: 
    int - C_FALSE if evidence is not found in the list, C_TRUE if found.
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
  Function: addEvidence
  Parameters:
    EvidenceList *list - Pointer to the list of evidence.
    EvidenceType evidence - The evidence type to be added to the list.
  Purpose: Adds a new evidence type to the evidence list if it does not already exist.
  Return: void
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
  Function: addEvidenceToRoom
  Parameters:
    Room *room - Pointer to the room where the evidence will be added.
    EvidenceType evidence - The evidence type to be added to the room's evidence list.
  Purpose: Adds a specified evidence type to the evidence list of a given room.
  Return: void
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
  Function: removeEvidenceFromRoom
  Parameters:
    Room *room - Pointer to the room from which the evidence will be removed.
    EvidenceType evidence - The evidence type to be removed from the room's evidence list.
  Purpose: Removes a specified evidence type from the evidence list of a given room.
  Return: void
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
  Function: cleanupEvidences
  Parameters:
    EvidenceList *list - Pointer to the evidence list to be cleaned up.
  Purpose: Frees the memory allocated for the evidence list and its elements.
  Return: void
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
