#include "defs.h"

Room *createRoom(const char *name) {
    Room *newRoom = (Room *)malloc(sizeof(Room));
    EvidenceList *list = (EvidenceList *)malloc(sizeof(EvidenceList));
    initEvidenceList(list);
    if (newRoom == NULL) {
        return NULL; // Memory allocation failed
    }
    strncpy(newRoom->name, name, MAX_STR);
    newRoom->connectedRooms = NULL;
    newRoom->connectedNum = 0;
    newRoom->evidenceList = list; // Init a empty evidence array
    newRoom->hunters = NULL; // Initially no hunters
    newRoom->ghost = NULL; // Initially no ghost
    sem_init(&newRoom->roomLock, 0, 1); // Initialize semaphore
    return newRoom;
}

// Function to connect two rooms
void connectRooms(Room *room1, Room *room2) {
    if (room1 == NULL || room2 == NULL) {
        // Handle null room pointers
        return;
    }
    printf("\n\nRoom connect: %s", room1->name);

    // Connect room1 to room2
    appendRoomToList(room1->connectedRooms, room2);
    room1->connectedNum++;
    

    // Connect room2 to room1
    appendRoomToList(room2->connectedRooms, room1);
    room2->connectedNum++;
}

void lockRoom(Room *room) {
    sem_wait(&room->roomLock);
}

void unlockRoom(Room *room) {
    sem_post(&room->roomLock);
}

void hunterSwitchRoom(HunterType *hunter, int roomIndex){
    lockRoom(hunter->currentRoom);
    
    //find room
    RoomNode *curr = hunter->currentRoom->connectedRooms;
    int ind = 0;
    while(roomIndex != ind){
        ind++;
        curr= curr->next;   
    }

    //move hunter to new room
    removeHunterFromRoom(hunter, hunter->currentRoom);
    addHunterToRoom(hunter, curr->room);

    //unlock the room
    unlockRoom(hunter->currentRoom);
}


//Add the hunter to the list of hunter in room
void addHunterToRoom(HunterType *hunter, Room *room){
    lockRoom(room);
    HunterNode *hunters = room->hunters;

    //create new hunter node
    HunterNode *newHunter = (HunterNode*) malloc(sizeof(HunterNode));
    newHunter->next = NULL;
    newHunter->hunter = hunter;

    //find tail of link list
    while(hunters->next != NULL){
        hunters = hunters ->next;
    }

    //add hunter to tail
    hunters->next = newHunter;
    hunter->currentRoom = room; 
    unlockRoom(room);
}

//Remove the hunter to the list of hunter in room
void removeHunterFromRoom(HunterType *hunter, Room *room){
    lockRoom(room);
    HunterNode *hunters = room->hunters;
    HunterNode *prev = NULL;

    while(strcmp(hunters->hunter->name, hunter->name)!= 0){
        prev = hunters;
        hunters = hunters ->next;
    }

    //check is hunter first
    if(prev == NULL){
        room->hunters = hunters->next;
    }else{
        prev->next = hunters->next;
    }
    //free node 
    free(hunters);
    unlockRoom(room);
}

// Function to append a room to a room's linked list of connected rooms
void appendRoomToList(RoomNode *connectHead, Room *roomToAdd) {
    RoomNode *newNode = (RoomNode *)malloc(sizeof(RoomNode));
    if (newNode == NULL) {
        // Handle memory allocation failure
        return;
    }

    newNode->room = roomToAdd;
    newNode->next = NULL;

    if (connectHead== NULL) {
        // First connection
        printf("\nHead is empty: %s", roomToAdd->name);
        connectHead = newNode;
    } else {
        // Append to the end of the list
        RoomNode *current = connectHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void cleanupRooms(RoomNode *rooms) {
    if(rooms == NULL) return ;
    RoomNode *curr = rooms;
    RoomNode *temp = curr->next;
    cleanupRoom(curr->room);
    while (temp != NULL) {
        curr = temp; 
        temp = temp->next;
        cleanupRoom(curr->room); 
    }
}

void cleanupRoom(Room *room) {
    if (room == NULL) return;

    // Free connectedRooms list node only
    RoomNode *curr = room->connectedRooms;
    RoomNode *temp = curr->next;
    free(curr);
    while (temp != NULL) {
        curr = temp; 
        temp = temp->next;
        free(curr); 
    }
    
    // Free evidenceList
    cleanupEvidences(room->evidenceList->head);

    // Free hunters list (if applicable)
    
    sem_destroy(&room->roomLock);
    free(room);
}