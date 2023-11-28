#include "defs.h"

Room *createRoom(const char *name) {
    Room *newRoom = (Room *)malloc(sizeof(Room));
    if (newRoom == NULL) {
        return NULL; // Memory allocation failed
    }
    strncpy(newRoom->name, name, MAX_STR);
    newRoom->connectedRooms = NULL;
    newRoom->evidenceList = NULL; // Initially no evidence
    newRoom->numEvidence = 0;
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

    // Connect room1 to room2
    appendRoomToList(room1, room2);

    // Connect room2 to room1
    appendRoomToList(room2, room1);
}

void lockRoom(Room *room) {
    sem_wait(&room->roomLock);
}

void unlockRoom(Room *room) {
    sem_post(&room->roomLock);
}


// Function to append a room to a room's linked list of connected rooms
void appendRoomToList(Room *currentRoom, Room *roomToAdd) {
    RoomNode *newNode = (RoomNode *)malloc(sizeof(RoomNode));
    if (newNode == NULL) {
        // Handle memory allocation failure
        return;
    }

    newNode->room = roomToAdd;
    newNode->next = NULL;

    if (currentRoom->connectedRooms == NULL) {
        // First connection
        currentRoom->connectedRooms = newNode;
    } else {
        // Append to the end of the list
        RoomNode *current = currentRoom->connectedRooms;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}


void cleanupRoom(Room *room) {
    if (room == NULL) return;
    // Free connectedRooms list
    // Free evidenceList
    // Free hunters list (if applicable)
    sem_destroy(&room->roomLock);
    free(room);
}