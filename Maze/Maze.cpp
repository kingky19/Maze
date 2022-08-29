// Tries to solve a maze using a recursive solution
// Shows path through the maze if one exists
// Group 6 Names (ENTER NAMES HERE): Adam Lynch, Brandon Wagoner, Emily Morales, Jacob Turnage, Kyle King

// '#define' was added

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
using namespace std;


const int FILENAMESIZE = 255;
const int MAXROWS = 22;
const int MAXCOLS = 81;


// ASCII characters to use for display
// https://theasciicode.com.ar/extended-ascii-code/block-graphic-character-ascii-code-219.html
const char WALL = (char)219;
const char WALLINFILE = '0';
const char START = 'S';
const char DEST = 'D';
const char EMPTY = ' ';
const char PATH = '*';
const char DEAD = 'X';
const bool DEBUG = false;

/*
* getFile will get a filename from the user. It then attmpts
* to open the file, read its contents, and store the maze
* structure into the maze array, based on the contents of the file.
* A WALLINFILE character in the file represents a maze wall, while
* an EMPTY character represents a free pathway. The file should also
* contain a single START character and a single DEST character.
* The function stops reading and exits the program if > MAXCOLS-1
* characters are found on one row, or if > MAXROWS rows are
* found in the file.  If the function is successful, then
* startX and startY will be set to the row and column number of
* where the START character is located in the maze.
*/
void getMaze(char maze[][MAXCOLS], int& startX, int& startY);

/*
* display is used to draw the maze on the screen.
*/
void display(const char maze[][MAXCOLS]);

/*
* Checks path clockwise to see if it is a wall, empty, dead end, dest, and start.
* The function takes in the maze and two integers.
* Checks N,S,E,W for every coordinate if it is possible to move to the next position.
* Returns true if next position is an acceptable then recursively calls again until DEST is found.
*/
bool checkPath(char maze[][MAXCOLS], int x, int y);

/*
* Goes back through and deletes the "X" and puts
* the "S" and the "D" in its original postion.
*/
void cleanMaze(char maze[][MAXCOLS]);

/*
* Used to initialize all positions in the maze
* array to the 'EMPTY' state
*/
void initMaze(char maze[][MAXCOLS]);

int main()
{
	char maze[MAXROWS][MAXCOLS];

	// This is where we wrote the main function
	// bool stop = false;

	int startX;
	int startY;

	initMaze(maze);
	getMaze(maze, startX, startY);
	display(maze);

	cout << "Press ENTER to solve maze.";

	char enterArray[10];
	cin.getline(enterArray, 10);

	bool solFound = checkPath(maze, startX, startY);
	maze[startX][startY] = START;
	cleanMaze(maze);
	display(maze);

	// Checks to see if the solution is true or false
	// True means it found a solution false means it did not.

	if (solFound)
	{
		cout << "Solution was found!\n";
	}
	else
	{
		cout << "There was no solution found.\n";
	}

	return 0;
}

void display(const char maze[][MAXCOLS])
{
	// system("cls") clears the screen; only works on Windows machines
	// You will probably need to comment this out if working on
	// a non-Windows machine

	system("cls");

	for (int row = 0; row < MAXROWS; row++)
	{
		cout << maze[row] << endl;
	}

	printf("\n");

}

void getMaze(char maze[][MAXCOLS], int& startX, int& startY)
{
	// This is where we wrote the getMaze function

	// Use 'strncat' (string concatenation) to add the ".txt" on the end of the name entered by use attempting to open file. 
	// DON'T let user enter too many characters, since we'll be adding additional characters to what they type
	// Syntax guide/example (from notes): 'strncat(char *dest, const char *src, size_t count)'

	ifstream inFile;
	int row;
	int col;
	char filename[FILENAMESIZE];
	char temparray[MAXCOLS];

	// This section contains our getFile code

	// This asks user to select a file, opens user's file choice, and 
	// checks to see if it actually opened.

	cout << "Which maze file do you want to use: (Do not enter .txt) \n";

	// The "minus 4" is used to accommodate for the ".txt".

	cin.getline(filename, FILENAMESIZE - 4);

	// This is putting ".txt" to the end of the user input for the maze,
	// then opens the desired file. 

	size_t len = strlen(filename);
	strncat(filename, ".txt", FILENAMESIZE - len);
	inFile.open(filename);

	if (!inFile)
	{
		cerr << "ERROR: Input file cannot be opened!\n";
		exit(1);
	}

	// Makes sure that there is a file actually there 
	// and applies the 'WALL' in the file to the 'START'.

	for (row = 0; row < MAXROWS && inFile.getline(temparray, MAXCOLS); row++)
	{
		int length = (int)strlen(temparray);
		for (col = 0; col < length; col++)
		{
			if (temparray[col] == WALLINFILE)
			{
				maze[row][col] = WALL;
			}
			else
			{
				maze[row][col] = temparray[col];	// Jacob: From maze eval., temparray has issue?

				if (temparray[col] == 'S')
				{
					// Stores start coordinates.

					startX = row;
					startY = col;
				}
			}
		}
	}

	// After file reading is finished, this code checks to see if the reading stopped
	// because of too many characters on a line in the file, or too many rows in the file
	// It is possible that you might have to change the conditions here depending on
	// exactly how you choose to read the file

	if (!inFile.eof())
	{
		if (row < MAXROWS)
		{
			cerr << "\nERROR: Line " << row + 1 << " in input file contains more than " << MAXCOLS-1 << " chars\n";
		}
		else
		{
			cerr << "\nERROR: There are more than " << MAXROWS << " lines in the file.\n";
		}
		inFile.close();
		exit(1);
	}
	inFile.close();
	cout << endl;
}

bool checkPath(char maze[][MAXCOLS], int x, int y)
{
	// (BONUS/EXTRA) Used for debugging to see steps/process in the terminal.
	/*if (DEBUG)
	{
		display(maze);
	}
	*/

	// This section of code checks all 4 directions from
	// every position and tries to generate 
	// a path through a maze.

	if (maze[x][y] == DEST)
	{
		return true;
	}

	if (maze[x][y] == EMPTY || maze[x][y] == START)
	{
		maze[x][y] = PATH;

		if (checkPath(maze, x + 1, y))
		{
			return true;
		}

		if (checkPath(maze, x, y + 1))
		{
			return true;
		}

		if (checkPath(maze, x - 1, y))
		{
			return true;
		}

		if (checkPath(maze, x, y - 1))
		{
			return true;
		}

		maze[x][y] = DEAD;
		return false;
	}
	else
	{
		return false;
	}
}

void initMaze(char maze[][MAXCOLS])
{
	// This is setting every position in the world to the empty state.

	for (int row = 0; row < MAXROWS; row++)
	{
		for (int col = 0; col < MAXCOLS - 1; col++)
		{
			maze[row][col] = ' ';
		}
	}

	for (int row = 0; row < MAXROWS; row++)
	{
		maze[row][MAXCOLS - 1] = '\0';
	}
}

void cleanMaze(char maze[][MAXCOLS])
{
	//This section of code cleans up the maze (removes 'X')

	for (int row = 0; row < MAXROWS; row++)
	{
		for (int col = 0; col < MAXCOLS - 1; col++)
		{
			if (maze[row][col] == DEAD)
			{
				maze[row][col] = EMPTY;
			}
		}
	}
}