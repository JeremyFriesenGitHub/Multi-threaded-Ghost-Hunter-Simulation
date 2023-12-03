# ## Jeremy Friesen && Matthew Hui
# ## 101268194 && 101261612

# # Final Project -- Threads, and Putting it All Together

This project involves building a multi-threaded ghost hunting simulation where hunters (threads) navigate through a network of connected rooms within a house, collecting different types of evidence to identify the specific ghost wandering around in the house. Hunters manage fear and boredom levels that influence their actions, while the ghost moves around leaving evidence and potentially ending its haunting if it becomes too bored.  

# ##List of Files

-ghost.c: Defines the behaviors of a ghost in the simulation, including its initialization, actions such as moving between rooms and leaving evidence, managing its boredom level, and placeholder cleanup functionality

-defs.h: Contains the definitions of the data structures, constants and the function prototypes used throughout the project.

-room.c: Manages room-related functionalities, including room creation, connections between rooms, and operations on room data structures within the ghost hunting simulation

-evidence.c: Handles evidence-related actions, such as adding or removing evidence from rooms, and manages the list or collection of evidence items that can be discovered by hunters

-house.c: Contains the implementation of the house setup, including initializing the house structure, populating it with rooms, and linking them to form the environment for the simulation

-hunter.c: Implements the behavior of hunters within the simulation, covering their movement, evidence collection, and the logic for fear and boredom levels affecting their actionss

-logger.c: Provides logging utilities that record and display the events and actions taken by both hunters and ghosts throughout the course of the simulation for debugging or analysis

-Makefile: Automates the compilation process, specifying how to compile and link the program's multiple source files (using 'make' command) and providing a 'clean' command to remove compiled outputs

-main.c: Contains the main code to execute

-utils.c: Contains helper functions to improve readability of the main function. 

-README.md: Contains this description

-github.png: Contains screenshot of the GitHub commit history

# ##Compiling and Executing

1. Navigate to the folder containing the source code in a terminal,

2. Use the command 'Make' in the terminal to compile all of the C programs in the directory. This will also create object files for the programs as well. 

3. Use the command 'valgrind --leak-check=full ./ghost_hunt_simulator'. while in the folder containing the executable to run the program. Optional use to run the valgrind check for leaks.

4. Use the command 'Make clean' to remove all of the object files created from the makefile, if necessary. 

5. Optional use to pipe in names using '< names.txt' or to use './validate' for validation checks. 

# ## Expected Output

1. While running standard version, user will be asked to input all of the names of the 4 different hunters. The threads will then become initialized. 

2. Upon completion, the simulation will run accordingly, where hunters and the ghost take at random action. The hunter can either review evidence, move to another room, or collect evidence in a room. The ghost can only move to another room or drop any evidence. 

3. If the hunters keep roaming around and do not find anything in the house, their boredom counter keeps on increasing and once they reach a max boredom, they leave the house and the thread ends. 

4. Similarly, the ghost also has a boredom counter, and leaves if they don't interact with hunters.

5. The Hunters have a fear counter as well, which increments if they are in the same room as the ghost. Simultaneously, the ghosts boredom counter is reset to zero. (Note: Ghosts cannot move while a hunter is in the same room). 

6. The simulation ends when every thread is terminated. That is, that everyone leaves the house, or when 

7. The results of the simulation are printed out at the bottom of the terminal, with either the ghost or the hunters winning. The hunters win if they are able to correctly review and identify the ghost with the evidence, and lose if they fail to do so.  

# ## Bonus Marks

Github Commit History: See screenshot file included. 
