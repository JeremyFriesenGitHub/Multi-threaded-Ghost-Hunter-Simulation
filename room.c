#include "defs.h"

/*
  Function: createRoom
  Parameters: const char *name - Name of the room to be created.
  Purpose: Allocates memory and initializes a new room with the specified name.
           Initializes an empty evidence list and sets initial values for room properties.
  Return: Pointer to the newly created Room object.
*/
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


/*
  Function: connectRooms
  Parameters: Room *room1 - First room to connect.
              Room *room2 - Second room to connect.
  Purpose: Establishes a bidirectional connection between two rooms.
           Adds each room to the other's list of connected rooms.
  Return: void
*/
void connectRooms(Room *room1, Room *room2) {
    if (room1 == NULL || room2 == NULL) {
        // Handle null room pointers
        return;
    }

    // Connect room1 to room2
    appendRoomToList(&room1->connectedRooms, room2);
    room1->connectedNum++;
    

    // Connect room2 to room1
    appendRoomToList(&room2->connectedRooms, room1);
    room2->connectedNum++;
}

/*
  Function: lockRoom
  Parameters: Room *room - The room to lock.
  Purpose: Acquires a semaphore lock on the given room for thread safety.
  Return: void
*/
void lockRoom(Room *room) {
    sem_wait(&room->roomLock);
}

/*
  Function: unlockRoom
  Parameters: Room *room - The room to unlock.
  Purpose: Releases the semaphore lock on the given room.
  Return: void
*/
void unlockRoom(Room *room) {
    sem_post(&room->roomLock);
}

/*
  Function: hunterSwitchRoom
  Parameters: HunterType *hunter - The hunter to move.
              int roomIndex - Index of the room to move the hunter to.
  Purpose: Moves a hunter from their current room to a new room based on the roomIndex.
  Return: void
*/
void hunterSwitchRoom(HunterType *hunter, int roomIndex){
    lockRoom(hunter->currentRoom);
    
    //find room
    RoomNode *curr = hunter->currentRoom->connectedRooms;
    int ind = 0;
    while(roomIndex != ind){
        ind++;
        curr= curr->next;   
    }
    
    
    //unlock the room
    unlockRoom(hunter->currentRoom);

    //move hunter to new room
    removeHunterFromRoom(hunter, hunter->currentRoom);
    hunter->currentRoom = curr -> room;
    addHunterToRoom(hunter, curr->room);
}


/*
  Function: addHunterToRoom
  Parameters: HunterType *hunter - The hunter to add to the room.
              Room *room - The room where the hunter will be added.
  Purpose: Adds a hunter to a room's list of hunters.
  Return: void
*/
void addHunterToRoom(HunterType *hunter, Room *room){
    lockRoom(room);
    HunterNode *hunters = room->hunters;

    //create new hunter node
    HunterNode *newHunter = (HunterNode*) malloc(sizeof(HunterNode));
    newHunter->next = NULL;
    newHunter->hunter = hunter;

    if(hunters != NULL){
            //find tail of link list
        while(hunters->next != NULL){
            hunters = hunters ->next;
        }
        //add hunter to tail
        hunters->next = newHunter;
        hunter->currentRoom = room; 
    }else{
        room->hunters = newHunter;
    }
    

    unlockRoom(room);
}

/*
  Function: removeHunterFromRoom
  Parameters: HunterType *hunter - The hunter to remove from the room.
              Room *room - The room where the hunter will be removed.
  Purpose: Removes a hunter from a room's list of hunters.
  Return: void
*/
void removeHunterFromRoom(HunterType *hunter, Room *room){
    lockRoom(room);
    HunterNode *hunters = room->hunters;
    HunterNode *prev = NULL;
    while(strcmp(hunters->hunter->name, hunter->name)!= 0 && hunters -> next != NULL){
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

/*
  Function: appendRoomToList
  Parameters: RoomNode **connectHead - Head of the connected rooms list.
              Room *roomToAdd - The room to be added to the list.
  Purpose: Adds a room to a linked list of connected rooms.
  Return: void
*/
void appendRoomToList(RoomNode **connectHead, Room *roomToAdd) {
    RoomNode *newNode = (RoomNode *)malloc(sizeof(RoomNode));
    if (newNode == NULL) {
        // Handle memory allocation failure
        return;
    }

    newNode->room = roomToAdd;
    newNode->next = NULL;

    if ((*connectHead)== NULL) {
        // First connection
        (*connectHead) = newNode;
    } else {
        // Append to the end of the list
        RoomNode *current = (*connectHead);
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

/*
  Function: cleanupRooms
  Parameters: RoomNode *rooms - Head of the list of rooms to be cleaned up.
  Purpose: Frees all memory associated with a linked list of rooms.
  Return: void
*/
void cleanupRooms(RoomNode *rooms) {
    if(rooms == NULL) return ;
    RoomNode *curr = rooms;
    RoomNode *temp = curr->next;
    cleanupRoom(curr->room);
    free(curr);
    while (temp != NULL) {
        curr = temp; 
        temp = temp->next;
        cleanupRoom(curr->room); 
        free(curr);
    }
}


/*
  Function: cleanupRoom
  Parameters: Room *room - The room to be cleaned up.
  Purpose: Frees all memory associated with a single room and its connected resources.
  Return: void
*/
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
    cleanupEvidences(room->evidenceList);
    cleanupHunters(room->hunters);

    // Free hunters list (if applicable)
    
    sem_destroy(&room->roomLock);
    free(room);
}