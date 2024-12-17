---How to run the game---
There are two ways to run the game:
1. Use of mingw32-make command.
--Explanation--
Go to the command line and open the address of the directory where the folder is located.
In my system, I had to type "mingw32-make" to compile my code but on other systems it may be just "make".
Then all of the code will me compiled and a file called "main.exe" will be produced.
To run that file, either just write "main" in the terminal or write "main.exe".
The program will start running.

2. Use of the compiler
--Explanation--
Go to the command line and open the address of the directory where the folder is located.
Type "gcc -o main main.c". This will compile the code.
To run that file, either just write "main" in the terminal or write "main.exe".
The program will start running.


---Gameplay & Game Logic---
Game Flow
At the start of the game, a welcome message is displayed and the name of the room is displayed. The player starts in the room called Arena. The health and strength of the user is also displayed in the start.

An array of room structs is used to store all of the rooms. The data of each room is read from the file for each room.

An array of creature structs is used to store the data for all of the creatures. The data of each creature is read from the file of each creature.

A variable called currentRoom is used to represent the room that the player is currently in. This is of type struct Room.

Then an infinite while loop starts running. Inside the while loop all of the game flow is defined. At first, the player is asked to write the command he wants to give. The command is then taken as input. Then the command is parsed. The player can either write a single-word command or a two-word command. And parsing parses the string into either one or two strings depending upon what type of command the user gives.

Once the parsing is done and the command is stored into variables. The execution of the command begins. For this I have used string comparison and if-else statements. Depending upon what the player entered, the program is executed. If the player has entered “move right”, the command is parsed into two strings “move” and “right”. If the first command is moved, we check what is the second command. If it is any of “up”, “down”, “right” and “left”, the player is moved into another room if there is a room. In this case, the player will be moved to the right if there is a room to the right of the current room. 

The infinite while loop runs until one of the three things happen:
The player uses the “exit” command to close the program.
The player is defeated by one of the enemies.
The player defeats all of the enemies and wins the game.

All of the code is written in one file. And many text files are used to store the game-related data. Data is read from and written to those text files whenever there have been a need. For adding features and capabilities to the program, functions have been used. All of the functions along with what they do have been explained in the report.