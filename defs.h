
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;
typedef struct RoomNode RoomNode;
typedef struct Room Room;
typedef struct Hunter HunterType; // Forward declaration
typedef struct Ghost GhostType; // Forward declaration


enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

//Structs
typedef struct Room {
    char name[MAX_STR]; // MAX_STR is a predefined constant
    RoomNode *connectedRooms; // Linked list of connected rooms
    EvidenceType *evidenceList; // Dynamically allocated array for evidence
    int numEvidence; // Number of evidence in the room
    sem_t evidenceMutex;
    HunterType *hunters; // Array or linked list of hunters in the room
    GhostType *ghost; // Pointer to a ghost, NULL if no ghost present
    sem_t roomLock; // Semaphore for threading synchronization
};

typedef struct RoomNode {
    struct Room *room;
    struct RoomNode *next;
} RoomNode;

typedef struct House {
    RoomNode *rooms; // Linked list of all rooms in the house
    HunterType *hunters; // Array or linked list of hunters
    EvidenceType *sharedEvidenceList; // Array or list of shared evidence
    int numHunters; // Number of hunters in the house
    int numSharedEvidence; // Number of evidence in shared list
} HouseType;

typedef struct Ghost {
    GhostClass type; // Enumerated type of the ghost
    Room *currentRoom; // Pointer to the current room the ghost is in
    int boredomTimer; // Boredom timer
    EvidenceType validEvidenceTypes[3]; // Array to store valid evidence types
} GhostType;

typedef struct Hunter {
    Room *currentRoom;
    EvidenceType equipmentType; // Type of evidence the hunter can collect
    char name[MAX_STR];
    EvidenceType *sharedEvidence; // Pointer to shared evidence collection
    int fear;
    int boredom;
} HunterType;


//function declarations
Room *createRoom(const char *name);
void connectRooms(Room *room1, Room *room2);
void addRoom(HouseType *house, Room *room);
void appendRoomToList(RoomNode *head, Room *room);
void leaveEvidence(GhostType *ghost);
void moveGhost(GhostType *ghost);

// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

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