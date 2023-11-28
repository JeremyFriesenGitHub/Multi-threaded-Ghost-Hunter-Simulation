# ## Jeremy Friesen && Matthew Hui
# ## 101268194 && 101261612

# # Final Project -- Threads, and Putting it All Together

This project involves building a multi-threaded ghost hunting simulation where hunters (threads) navigate through a network of connected rooms within a house, collecting different types of evidence to identify the specific ghost wandering around in the house. Hunters manage fear and boredom levels that influence their actions, while the ghost moves around leaving evidence and potentially ending its haunting if it becomes too bored.  

# ##List of Files

-ghost.c: Defines the behaviors of a ghost in the simulation, including its initialization, actions such as moving between rooms and leaving evidence, managing its boredom level, and placeholder cleanup functionality.
-defs.h: Contains the definitions of the data structures, constants and the function prototypes used throughout the project.
-room.c: Manages room-related functionalities, including room creation, connections between rooms, and operations on room data structures within the ghost hunting simulation.
-evidence.c: Handles evidence-related actions, such as adding or removing evidence from rooms, and manages the list or collection of evidence items that can be discovered by hunters.
-house.c: Contains the implementation of the house setup, including initializing the house structure, populating it with rooms, and linking them to form the environment for the simulation.
-hunter.c:Implements the behavior of hunters within the simulation, covering their movement, evidence collection, and the logic for fear and boredom levels affecting their actions.
-logger.c: Provides logging utilities that record and display the events and actions taken by both hunters and ghosts throughout the course of the simulation for debugging or analysis.
-Makefile: Automates the compilation process, specifying how to compile and link the program's multiple source files (using 'make' command) and providing a 'clean' command to remove compiled outputs.
-main.c: Contains the main code to execute
-utils.c: Contains helper functions to improve readability of the main function. 
-README.md: Contains this description

# ##Compiling and Executing

1. Navigate to the folder containing the source code in a terminal,
2. Use the command 'Make' in the terminal to compile all of the C programs in the directory. This will also create object files for the programs as well. 
3. Use the command 'valgrind --leak-check=full ./ghost_hunt_simulator'. while in the folder containing the executable to run the program. Optional use to run the valgrind check for leaks.
4. Use the command 'Make clean' to remove all of the object files created from the makefile, if necessary.  

# ## Expected Output

1. Displaying the Main Menu: Upon running the program, it displays a main menu with options to print rooms, print ghosts, print ghosts by likelihood, print most/least likely ghosts, or exit.

2. User Interaction: The user selects an option by entering a corresponding number. This choice is processed to execute the desired action.

3. Output Based on Selection:

If "Print rooms" (or '1') is selected, the program outputs details of all rooms and the ghosts associated with each room.
If "Print ghosts" (or '2') is chosen, it lists all ghosts in the building.
Selecting "Print ghosts by likelihood" (or '3') displays ghosts sorted by their likelihood scores.
Choosing "Print most/least likely ghosts" (or '4') shows the ghosts with the highest and lowest likelihood scores.
If "Exit" is selected (or '0'), the program terminates.
If any other input is inputted, the system prints out an invalid selection error and lets the user try to input again. 

4. Memory Cleanup: Before exiting, the program performs a cleanup, freeing all dynamically allocated memory to prevent memory leaks.

5. Program Termination: Finally, the program ends, typically returning a 0 exit code to indicate successful completion.

# ## Bonus Marks

Github SS:
