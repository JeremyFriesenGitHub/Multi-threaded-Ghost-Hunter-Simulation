#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     50
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     3500
#define GHOST_WAIT      1500
#define NUM_HUNTERS     4
#define FEAR_MAX        25
#define LOGGING         C_TRUE
#define MAX_EVIDENCE    4


typedef enum EvidenceType EvidenceType;
typedef struct EvidenceNode EvidenceNode;
typedef enum GhostClass GhostClass;
typedef struct RoomNode RoomNode;
typedef struct Room Room;
typedef struct HunterNode HunterNode; 
typedef struct Hunter HunterType; // Forward declaration
typedef struct Ghost GhostType; // Forward declaration
typedef struct EvidenceList EvidenceList;


enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };


//Structs
typedef struct Room {
    char name[MAX_STR]; // MAX_STR is a predefined constant
    RoomNode *connectedRooms; // Linked list of connected rooms
    int connectedNum;  //Amount of connect room
    EvidenceList *evidenceList; // Dynamically allocated array for evidence
    HunterNode *hunters; // Array or linked list of hunters in the room
    GhostType *ghost; // Pointer to a ghost, NULL if no ghost present
    sem_t roomLock; // Semaphore for threading synchronization
}Room;

typedef struct HunterNode {
    struct Hunter *hunter;
    struct HunterNode *next;
} HunterNode;

typedef struct RoomNode {
    struct Room *room;
    struct RoomNode *next;
} RoomNode;

typedef struct EvidenceNode {
    EvidenceType evidence;
    struct EvidenceNode *next;
} EvidenceNode;

typedef struct EvidenceList {
    struct EvidenceNode *head;
    struct EvidenceNode *tail;
    sem_t evidenceMutex;
    int numEvidence; // Number of evidence in the room
} EvidenceList;


typedef struct House {
    struct RoomNode *rooms; // Linked list of all rooms in the house
    struct HunterNode *hunters; // Array or linked list of hunters
    struct EvidenceList *sharedEvidenceList; // Array or list of shared evidence
    int numHunters; // Number of hunters in the house
} HouseType;

typedef struct Ghost {
    GhostClass type; // Enumerated type of the ghost
    Room *currentRoom; // Pointer to the current room the ghost is in
    int boredomTimer; // Boredom timerSS
    EvidenceType validEvidenceTypes[3]; // Array to store valid evidence types
} GhostType;

typedef struct Hunter {
    Room *currentRoom;
    EvidenceType equipmentType; // Type of evidence the hunter can collect
    char name[MAX_STR];
    EvidenceList *sharedEvidence; // Pointer to shared evidence collection
    int fear;
    int boredom;
} HunterType;


//init Functions
void initEvidenceList(EvidenceList *list);
void initHouse(HouseType *house);
void initHunter(HunterType *hunter, const char *name, Room *startingRoom, EvidenceType equipmentType, EvidenceList *sharedEvidence);
void initGhost(GhostType *ghost, GhostClass type, Room *startingRoom);

//function declarations
bool canAddEvidence(EvidenceList *list, EvidenceType evidence);
void replaceOldestEvidence(Room *room, EvidenceType newEvidence);
Room *createRoom(const char *name);
void connectRooms(Room *room1, Room *room2);
void addRoom(RoomNode **rooms, Room *room);
void appendRoomToList(RoomNode **head, Room *room);
void leaveEvidence(GhostType *ghost);
void moveGhost(GhostType *ghost);
void populateRooms(HouseType* house);
void addEvidenceToRoom(Room *room, EvidenceType evidence);
void moveHunter(HunterType *hunter);
void collectEvidence(HunterType *hunter);
void reviewEvidence(HunterType *hunter);
void checkHunterFearAndBoredom(HunterType *hunter);
void addEvidence(EvidenceList *list, EvidenceType evidence);  //add evidence to list
void hunterSwitchRoom(HunterType *hunter, int roomIndex); //move hunter to new room
void addHunterToRoom(HunterType *hunter, Room *room); //add hunter to room
void removeHunterFromRoom(HunterType *hunter, Room *room); //remove hunter from room
void lockRoom(Room *room);
void unlockRoom(Room *room);
void checkGhostBoredom(GhostType *ghost);
void initHunterNode(HunterNode *node, HunterType *hunter);
void* ghostFunction(void *arg);
void* hunterFunction(void *arg);
void hunterAction(HunterType *hunter);
void ghostAction(GhostType *ghost);

//cleanup function
void cleanupRooms(RoomNode *);
void cleanupRoom(Room *);
void cleanupHunter(HunterType *);
void cleanUpGhost(GhostType *);
void cleanupEvidences(EvidenceList *);
void cleanupHunters(HunterNode *);
void cleanUpHouse(HouseType *house);


// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter
int getEvidence(EvidenceList *, EvidenceType); //give true and false depend on does the evidence exist

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);
