#include "defs.h"

/*
    Dynamically allocates several rooms and populates the provided house.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
/*
  Function: populateRooms(HouseType* )
  Purpose:  This function will populate the house
        in     HouseType* house     pointer to the house
  return void
*/
void populateRooms(HouseType* house) {
    // First, create each room

    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    struct Room* van                = createRoom("Van");
    struct Room* hallway            = createRoom("Hallway");
    struct Room* master_bedroom     = createRoom("Master Bedroom");
    struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
    struct Room* bathroom           = createRoom("Bathroom");
    struct Room* basement           = createRoom("Basement");
    struct Room* basement_hallway   = createRoom("Basement Hallway");
    struct Room* right_storage_room = createRoom("Right Storage Room");
    struct Room* left_storage_room  = createRoom("Left Storage Room");
    struct Room* kitchen            = createRoom("Kitchen");
    struct Room* living_room        = createRoom("Living Room");
    struct Room* garage             = createRoom("Garage");
    struct Room* utility_room       = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(&house->rooms, van);
    addRoom(&house->rooms, hallway);
    addRoom(&house->rooms, master_bedroom);
    addRoom(&house->rooms, boys_bedroom);
    addRoom(&house->rooms, bathroom);
    addRoom(&house->rooms, basement);
    addRoom(&house->rooms, basement_hallway);
    addRoom(&house->rooms, right_storage_room);
    addRoom(&house->rooms, left_storage_room);
    addRoom(&house->rooms, kitchen);
    addRoom(&house->rooms, living_room);
    addRoom(&house->rooms, garage);
    addRoom(&house->rooms, utility_room);
}

/*
  Function: initHouse
  Parameters:
    HouseType *house - Pointer to the house that will be initialized.
  Purpose: Initializes the house by allocating memory for the shared evidence list and setting up initial values for house properties.
  Return: void
*/
void initHouse(HouseType *house) {
    EvidenceList *list = (EvidenceList *) malloc(sizeof(EvidenceList));
    initEvidenceList(list);
    house->rooms = NULL;
    house->hunters = NULL;
    house->sharedEvidenceList = list;
    house->numHunters = 0;
}

/*
  Function: addRoom
  Parameters:
    RoomNode **rooms - Pointer to the head of the room list.
    Room *room - Pointer to the room that will be added to the list.
  Purpose: Adds a room to the house's room list. Creates a new node for the room and appends it to the list.
  Return: void
*/
void addRoom(RoomNode **rooms, Room *room) {
    if (room == NULL) return;
    RoomNode *newNode = (RoomNode *)malloc(sizeof(RoomNode));
    newNode->room = room;
    newNode->next = NULL;
    if ((*rooms) == NULL){
        (*rooms) = newNode;
    }else{
        RoomNode *current = (*rooms);
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

/*
  Function: cleanUpHouse
  Parameters:
    HouseType *house - Pointer to the house that will be cleaned up.
  Purpose: Frees all allocated memory associated with the house, including rooms, hunters, and shared evidence list.
  Return: void
*/
void cleanUpHouse(HouseType *house) {
    if (house == NULL) return;

    // Free each room in the house's room list
    cleanupRooms(house->rooms);
    // Free hunters and sharedEvidenceList
    cleanupHunters(house->hunters);
    cleanupEvidences(house->sharedEvidenceList);
    // Set all pointers to NULL and counters to 0
}
