//=====================================================================================================================================================
// Name        : game.cpp
// Author      : Muhammad Shaheer
// Roll no.    : 19I-2152
// Section     : CS-E
// Description : Semester Project (Game of life)
//=====================================================================================================================================================

#include <iostream>
#include <fstream>

using namespace std;

// All functions prototypes declarations

void start();															// For starting the game (Is called in "main()")

char menu();															// For displaying menu to the user (Introduction about the game and rules)

int readFromfile(int&, int&, int[][2], int&);							// For reading data from the file to start the game (Is called in "start()")

void aliveGrid(int[][20], int[][2], int);								// For converting grid to alive state for every generation (Every display)

void printGrid(int[][20]);												// For printing the grid after every generation (Is called after every generation for printng new grid)

void defaultGrid(int[][20], int[][2], int);								// For converting grid to default state after every generation (Every display)

void simulation(int[][20], int, int[][2], int, int[][2], int);			// For game simulation i.e generations (Is called in "start()")

int neighbourAlive(int[][20], int, int);								// For counting all alive neighbour cells (Is called to count alive neighbours after every generation)

void displaySec_array(int[][2], int);									// For displaying second array with alive cells after every generation

int fill_neiArray(int[][20], int[][2], int, int, int);					// For filling nei array with dead neighbour cells of an alive cell after every generation

void displayNei_array(int[][2], int);									// For displaying the neighbour array in which dead neighbours are stored

void writeTofile(int[][2], int, int, int);								// For writing the output to file after every generation

// Main function

int main()
{
	start(); // Starting the game

	return 0;
}

// For starting the game (Is called in "main()")

void start() {

	char continueGame;

	continueGame = menu(); // Displaying game menu to the user

	if (continueGame == 'y' || continueGame == 'Y') { // If user wants to continue playing the game

		// Variable declarations

		int grid[20][20] = { 0 };				// Main grid for game simulation
		int secArray[100][2] = { 0 };			// Declaring a second array to store all the alive cells after every generation
		int neiArray[300][2] = { 0 };			// Declaring a neighbour array to store neighbours of all the alive cells
		char start;							// To ask user to start the game or not
		int numOfgen;						// For storing number of generations
		int numOfcells;						// For storing number of alive cells at start of game
		int x;								// For storing the value of x-coordinate
		int y;								// For storing the value of y-coordinate
		int fileOpen;						// For verifying that file is opened or not
		int lastOfsec = -1;					// For storing last index of second array

		fileOpen = readFromfile(numOfgen, numOfcells, secArray, lastOfsec); // Reading data from the file to start the game

		if (fileOpen == 1) { // If file is open
			cout << "\nNumber of generations (Default) : " << numOfgen << endl; // Displaying number of generations at startup
			cout << "\nGeneration 0 : " << endl;

			aliveGrid(grid, secArray, lastOfsec); // Making grid alive i.e. all the elements to "1"

			printGrid(grid); // Displaying grid when it is ready

			cout << "\nNumber of alive cells : " << numOfcells << endl; // Displaying total number alive cells in the grid at startup

			displaySec_array(secArray, lastOfsec); // Displaying all alive cells in grid by the aid of second array

			// Filling the neighbour array by taking alive cell coordinates from second array

			int indexOfnei;
			int lastOfnei = 0;

			for (int i = 0; i < 100; i++) { // To control the index of second array
				x = secArray[i][0];
				y = secArray[i][1];
				indexOfnei = fill_neiArray(grid, neiArray, x, y, lastOfnei); // Filling neighbour array with dead neighbours
				lastOfnei = indexOfnei; // Storing the index of neighbour array in a variable to start storing next dead neighbours from the last index

				if (i == lastOfsec) { // Loop breaks because further elements are "0"
					break;
				}
			}

			displayNei_array(neiArray, lastOfnei); // Displaying all dead neighbour cells by the aid of neighbour array

			defaultGrid(grid, secArray, lastOfsec); // Making grid default i.e. all the elements to "0"

			// After game setup is done

			cout << "\nGame setup is ready do you want to start the game? [y/n] ";
			cin >> start; // Taking input from user to start the game or exit

			// Continue taking input if user enters other than "y" or "Y" for yes and "n" or "N" for no

			if (start != 'y' && start != 'Y' && start != 'n' && start != 'N') {

				do {
					cout << "Invalid Input!" << endl;
					cout << "Enter again : ";
					cin >> start;
				}

				while (start != 'y' && start != 'Y' && start != 'n' && start != 'N');
			}

			if (start == 'y' || start == 'Y') { // If user enters "y" or "Y" for yes

				// Starting game simulation i.e generations

				simulation(grid, numOfgen, secArray, lastOfsec, neiArray, lastOfnei);
			}

			else if (start == 'n' || start == 'N') { // If user enters "n" or "N" for no
				cout << "\nExiting...\n" << endl;
			}
		}

		else if (fileOpen == 0) { // If file does not open
			cout << "\nError opening file.\n" << endl;
		}
	}

	else if (continueGame == 'n' || continueGame == 'N') { // If user does not want to continue playing game
		cout << "\nExiting...\n" << endl;
	}
}

// For displaying menu to the user (Introduction about the game and rules)

char menu() {

	char continueGame;

	// Displaying menu to the player

	cout << "\nTHE GAME OF LIFE" << endl; // Name of the game

	// Introduction to the game

	cout << "\nIntroduction :\n\tThe game of life, also known simply as life, is a cellular automaton devised by the"
		<< "\n\tBritish mathematician John Horton Conway in 1970." << endl;

	// Rules of the game

	cout << "\nRules of game :\n\tThe universe of the Game of life is an infinite two-dimensional orthogonal grid of square cells,"
		<< "\n\teach of which is in one of two possible states, alive or dead."
		<< "\n\n\tEvery cell interacts with its eight neighbours, which are the cells that are horizontally,"
		<< "\n\tvertically, or diagonally adjacent."
		<< "\n\n\tIn every generation, the following transitions occur :"
		<< "\n\n\t1. Any alive cell with two or three alive neighbours lives on to the next generation."
		<< "\n\t2. Any alive cell with more than three alive neighbours dies, as if by over-population."
		<< "\n\t3. Any alive cell with fewer than two alive neighbours dies, as if caused by under-population."
		<< "\n\t4. Any dead cell with exactly three alive neighbours becomes alive cell, as if by reproduction." << endl;

	// Display information

	cout << "\nDisplay information :"
		<< "\n\t1 - Alive cell"
		<< "\n\t. - Dead cell"
		<< "\n\tSec array - holds the alive cells of current generation"
		<< "\n\tNei array - holds the dead neighbours of all alive cellss" << endl;

	cout << "\nDo you want to continue playing game [y/n] "; // Asking if user wants to continue to the game
	cin >> continueGame;

	// Continue taking input if user enters other than "y" or "Y" for yes and "n" or "N" for no

	if (continueGame != 'y' && continueGame != 'Y' && continueGame != 'n' && continueGame != 'N') {

		do {
			cout << "Invalid Input!" << endl;
			cout << "Enter again : ";
			cin >> continueGame;
		}

		while (continueGame != 'y' && continueGame != 'Y' && continueGame != 'n' && continueGame != 'N');
	}

	return continueGame;
}

// For reading data from the file to start the game (Is called in "start()")

int readFromfile(int& gen, int& cells, int secArray[100][2], int& last_secArray) {

	string fileName; // For storing name of the file

	fstream ifile; // Object creation (Reading object)

	cout << "\nEnter file name to read data : "; // Taking file name as input from the user
	cin >> fileName;

	// Opening the file to read from

	ifile.open(fileName, ios::in);

	if (ifile) { // If file is opened succesfully
		ifile >> gen; // Taking number of generations as input fron the file

		ifile >> cells; // Taking number of cells alive at start of the geme as input fron the file

		// Reading coordinates of alive cells from the file and storing them in second array

		int x; // x-coordinate of cell
		int y; // y-coordinate of cell
		int i = 0;
		int n = 0;

		// Taking alive cell coordinates from the file and storing them in second array

		while (n < cells) {
			ifile >> x >> y; // Taking coordinates as input from the file
			secArray[i][0] = x;
			secArray[i][1] = y;
			i++;
			n++;
		}

		last_secArray = last_secArray + i; // Calculating last of second array

		ifile.close(); // Closing the file

		return 1;
	}

	else { // If file does not open
		return 0;
	}
}

// For converting grid to alive state for every generation (Every display)

void aliveGrid(int grid[20][20], int secArray[100][2], int lastOfsec) {

	int x; // For storing the value of x-coordinate
	int y; // For storing the value of y-coordinate

	for (int i = 0; i < 100; i++) { // To control the index of second array
		x = secArray[i][0];
		y = secArray[i][1];
		grid[x][y] = 1; // Making cells alive in grid to make grid alive

		if (i == lastOfsec) { // Loop breaks because further elements are "0"
			break;
		}
	}
}

// For printing the grid after every generation (Is called after every generation for printng new grid)

void printGrid(int grid[20][20]) {

	cout << "\nGrid :" << endl;

	for (int x = 0; x < 20; x++) { // To control index of rows of grid

		for (int y = 0; y < 20; y++) { // To control index of columns of grid

			if (grid[x][y] == 1) { // If the cell is alive print "1"
				cout << "1" << " ";
			}

			else { // If the cell is dead print dot
				cout << "." << " ";
			}
		}

		cout << endl;
	}
}

// For converting grid to default state after every generation (Every display)

void defaultGrid(int grid[20][20], int secArray[100][2], int lastOfsec) {

	int x; // For storing the value of x-coordinate
	int y; // For storing the value of y-coordinate

	for (int i = 0; i < 100; i++) { // To control the index of second array
		x = secArray[i][0];
		y = secArray[i][1];
		grid[x][y] = 0; // Making cells alive in grid to make grid alive

		if (i == lastOfsec) { // Loop breaks because further elements are "0"
			break;
		}
	}
}

// For game simulation i.e generations (Is called in "start()")

void simulation(int grid[20][20], int numOfgen, int secArray[100][2], int lastOfsec, int neiArray[300][2], int lastOfnei) {

	int n = 0;							// For controlling loop of generations count
	int neiAlive = 0;					// For storing the count of alive neighbours
	int sec_Array[100][2] = { 0 };		// For storing the elements of second array
	int last_sec;						// For storing the last index of second array
	int indexOfdead;					// For storing the index of cell which dies and is deleted from second array
	int indexOfalive;					// For storing the index of cell which lives and is deleted from neighbour array
	int wholeCoordinate;				// For making both x and y coordinates as one and storing it in 1D second array and 1D neighbour array
	int temp;							// Temporary variable for storing x and y coordinates after separation as a whole coordinate
	int x;								// For storing the value of x-coordinate
	int y;								// For storing the value of y-coordinate
	int totalAlive;						// For counting total number of alive cells in every generation

	while (n < numOfgen) { // To control number of generations to be performed
		cout << "\nGeneration " << n + 1 << " :" << endl; // Displaying generation count

		for (int i = 0; i < 100; i++) { // To control the index of second array
			sec_Array[i][0] = secArray[i][0];
			sec_Array[i][1] = secArray[i][1];
		}

		last_sec = lastOfsec;

		aliveGrid(grid, sec_Array, lastOfsec); // Making grid alive i.e. all the elements to "1"

		for (int i = 0; i < 100; i++) { // To control the index of second array
			x = secArray[i][0];
			y = secArray[i][1];
			neiAlive = neighbourAlive(grid, x, y); // Counting alive neighbours of current alive cell

			if (neiAlive != 2 && neiAlive != 3) { // If count of alive neighbours is less then two or greater than three
				indexOfdead = i;

				// Deletion of dead cells from the second array and array shifting of remaining alive cells to right side

				for (int j = indexOfdead; j < 100; j++) {
					secArray[j][0] = secArray[j + 1][0];
					secArray[j][1] = secArray[j + 1][1];
				}

				lastOfsec--; // Last of second array decreases as dead cells are being removed from second array
				i--;
			}

			if (i == lastOfsec) { // Loop breaks because further elements are "0"
				break;
			}
		}

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array
			x = neiArray[i][0];
			y = neiArray[i][1];
			neiAlive = neighbourAlive(grid, x, y); // Counting alive neighbours of current alive cell

			if (neiAlive == 3) { // If count of alive neighbours is equal to three

				// Insertion of new alive cells to the second array after previously stored alive cells

				lastOfsec++;
				secArray[lastOfsec][0] = neiArray[i][0];
				secArray[lastOfsec][1] = neiArray[i][1];
			}

			if (i == lastOfnei - 1) { // Loop breaks because further elements are "0"
				break;
			}
		}

		defaultGrid(grid, sec_Array, last_sec); // Making grid default i.e. all the elements to "0"

		aliveGrid(grid, secArray, lastOfsec); // Making grid alive i.e. all the elements to "1"

		printGrid(grid); // Printing the grid for current generation

		// Calculating total number of alive cells for current generation

		totalAlive = 0;

		for (int i = 0; i < 100; i++) { // To control the index of second array

			totalAlive++;

			if (i == lastOfsec) { // Loop breaks because further elements are "0"
				break;
			}
		}

		cout << "\nNumber of alive cells : " << totalAlive << endl; // Displaying total number of alive cells for current generation

		displaySec_array(secArray, lastOfsec); // Displaying second array (Alive cells) for current generation

		// Initializing neighbour array to zero before filling with dead neighbours

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array
			neiArray[i][0] = 0;
			neiArray[i][1] = 0;
		}

		lastOfnei = 0;

		// Filling the neighbour array by taking alive cell coordinates from second array

		int indexOfnei;
		int lastOfnei = 0;

		for (int i = 0; i < 100; i++) { // To control the index of second array			
			x = secArray[i][0];
			y = secArray[i][1];
			indexOfnei = fill_neiArray(grid, neiArray, x, y, lastOfnei); // Filling neighbour array with dead neighbours
			lastOfnei = indexOfnei; // Storing the index of neighbour array in a variable to start storing next dead neighbours from the last index

			if (i == lastOfsec) { // Loop breaks because further elements are "0"
				break;
			}
		}

		displayNei_array(neiArray, lastOfnei); // Displaying all dead neighbour cells by the aid of neighbour array

		defaultGrid(grid, secArray, lastOfsec); // Making grid default i.e. all the elements to "0"

		writeTofile(secArray, lastOfsec, numOfgen, totalAlive); // Writing data to output file

		n++;
	}

	cout << endl;
}

// For counting all alive neighbour cells (Is called to count alive neighbours after every generation)

int neighbourAlive(int grid[20][20], int x, int y) {

	int countAlive = 0;

	if (grid[x - 1][y - 1] == 1) { // 1st neighbour
		countAlive++;
	}

	if (grid[x - 1][y] == 1) { // 2nd neighbour
		countAlive++;
	}

	if (grid[x - 1][y + 1] == 1) { // 3rd neighbour
		countAlive++;
	}

	if (grid[x][y - 1] == 1) { // 4th neighbour
		countAlive++;
	}

	if (grid[x][y + 1] == 1) { // 5th neighbour
		countAlive++;
	}

	if (grid[x + 1][y - 1] == 1) { // 6th neighbour
		countAlive++;
	}

	if (grid[x + 1][y] == 1) { // 7th neighbour
		countAlive++;
	}

	if (grid[x + 1][y + 1] == 1) { // 8th neighbour
		countAlive++;
	}

	return countAlive;
}

// For displaying second array with alive cells after every generation

void displaySec_array(int secArray[100][2], int lastOfsec) {

	int x; // For storing the value of x-coordinate
	int y; // For storing the value of y-coordinate

	cout << "\nSec-Array : ";

	for (int i = 0; i < 100; i++) { // To control the index of second array
		x = secArray[i][0];
		y = secArray[i][1];

		cout << "(" << x << "," << y << ")" << " ";

		if (i == lastOfsec) { // Loop breaks because further elements are "0"
			break;
		}
	}

	cout << endl;

	cout << "Last : " << lastOfsec << endl; // Displaying the last index of second array in which an alive cell is stored
}

// For filling nei array with dead neighbour cells of an alive cell after every generation

int fill_neiArray(int grid[][20], int neiArray[300][2], int x, int y, int n) {

	int count = 0; // For checking duplication of cells in neighbour array

	if (grid[x - 1][y - 1] == 0) { // 1st Neighbour

		// Ensuring that there is no duplication of dead cells in neighbour array 

		count = 0;

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array

			if (neiArray[i][0] == x - 1 && neiArray[i][1] == y - 1) { // Checking if current coordinates are already present in neighbour array
				count++;
			}

			if (i == n) { // Loop breaks because further elements are "0"
				break;
			}
		}

		if (count == 0) { // If there is no duplication i.e. count is "0" cell is inserted in neighbour array
			neiArray[n][0] = x - 1;
			neiArray[n][1] = y - 1;
			n++;
		}
	}

	if (grid[x - 1][y] == 0) { // 2nd Neighbour

		// Ensuring that there is no duplication of dead cells in neighbour array 

		count = 0;

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array

			if (neiArray[i][0] == x - 1 && neiArray[i][1] == y) { // Checking if current coordinates are already present in neighbour array
				count++;
			}

			if (i == n) { // Loop breaks because further elements are "0"
				break;
			}
		}

		if (count == 0) { // If there is no duplication i.e. count is "0" cell is inserted in neighbour array
			neiArray[n][0] = x - 1;
			neiArray[n][1] = y;
			n++;
		}
	}

	if (grid[x - 1][y + 1] == 0) { // 3rd Neighbour

		// Ensuring that there is no duplication of dead cells in neighbour array 

		count = 0;

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array

			if (neiArray[i][0] == x - 1 && neiArray[i][1] == y + 1) { // Checking if current coordinates are already present in neighbour array
				count++;
			}

			if (i == n) { // Loop breaks because further elements are "0"
				break;
			}
		}

		if (count == 0) { // If there is no duplication i.e. count is "0" cell is inserted in neighbour array
			neiArray[n][0] = x - 1;
			neiArray[n][1] = y + 1;
			n++;
		}
	}

	if (grid[x][y - 1] == 0) { // 4th Neighbour

		// Ensuring that there is no duplication of dead cells in neighbour array 

		count = 0;

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array

			if (neiArray[i][0] == x && neiArray[i][1] == y - 1) { // Checking if current coordinates are already present in neighbour array
				count++;
			}

			if (i == n) { // Loop breaks because further elements are "0"
				break;
			}
		}

		if (count == 0) { // If there is no duplication i.e. count is "0" cell is inserted in neighbour array
			neiArray[n][0] = x;
			neiArray[n][1] = y - 1;
			n++;
		}
	}

	if (grid[x][y + 1] == 0) { // 5th Neighbour

		// Ensuring that there is no duplication of dead cells in neighbour array 

		count = 0;

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array

			if (neiArray[i][0] == x && neiArray[i][1] == y + 1) { // Checking if current coordinates are already present in neighbour array
				count++;
			}

			if (i == n) { // Loop breaks because further elements are "0"
				break;
			}
		}

		if (count == 0) { // If there is no duplication i.e. count is "0" cell is inserted in neighbour array
			neiArray[n][0] = x;
			neiArray[n][1] = y + 1;
			n++;
		}
	}

	if (grid[x + 1][y - 1] == 0) { // 6th Neighbour

		// Ensuring that there is no duplication of dead cells in neighbour array 

		count = 0;

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array

			if (neiArray[i][0] == x + 1 && neiArray[i][1] == y - 1) { // Checking if current coordinates are already present in neighbour array
				count++;
			}

			if (i == n) { // Loop breaks because further elements are "0"
				break;
			}
		}

		if (count == 0) { // If there is no duplication i.e. count is "0" cell is inserted in neighbour array
			neiArray[n][0] = x + 1;
			neiArray[n][1] = y - 1;
			n++;
		}
	}

	if (grid[x + 1][y] == 0) { // 7th Neighbour

		// Ensuring that there is no duplication of dead cells in neighbour array 

		count = 0;

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array

			if (neiArray[i][0] == x + 1 && neiArray[i][1] == y) { // Checking if current coordinates are already present in neighbour array
				count++;
			}

			if (i == n) { // Loop breaks because further elements are "0"
				break;
			}
		}

		if (count == 0) { // If there is no duplication i.e. count is "0" cell is inserted in neighbour array
			neiArray[n][0] = x + 1;
			neiArray[n][1] = y;
			n++;
		}
	}

	if (grid[x + 1][y + 1] == 0) { // 8th Neighbour

		// Ensuring that there is no duplication of dead cells in neighbour array 

		count = 0;

		for (int i = 0; i < 300; i++) { // To control the index of neighbour array

			if (neiArray[i][0] == x + 1 && neiArray[i][1] == y + 1) { // Checking if current coordinates are already present in neighbour array
				count++;
			}

			if (i == n) { // Loop breaks because further elements are "0"
				break;
			}
		}

		if (count == 0) { // If there is no duplication i.e. count is "0" cell is inserted in neighbour array
			neiArray[n][0] = x + 1;
			neiArray[n][1] = y + 1;
			n++;
		}
	}

	return n;
}

// For displaying the neighbour array in which dead neighbours are stored

void displayNei_array(int neiArray[300][2], int lastOfnei) {

	int x; // For storing the value of x-coordinate
	int y; // For storing the value of y-coordinate

	// Displaying neighbour array (Dead neighbours)

	cout << "\nNei-Array : ";

	for (int i = 0; i < 300; i++) { // To control the index of neighbour array
		x = neiArray[i][0];
		y = neiArray[i][1];

		cout << "(" << x << "," << y << ")" << " ";

		if (i == lastOfnei - 1) { // Loop breaks because further elements are "0"
			break;
		}
	}

	cout << endl;

	cout << "Last : " << lastOfnei - 1 << endl; // Displaying last index of neighbour array in which a dead cell is stored
}

// For writing the output to file after every generation

void writeTofile(int secArray[100][2], int lastOfsec, int numOfgen, int totalAlive) {

	int x; // For storing the value of x-coordinate
	int y; // For storing the value of y-coordinate

	fstream ofile; // Object creation (Writing object)

	// Opening the file to write on

	ofile.open("Output.txt", ios::out);

	ofile << numOfgen << endl; // Writing number of generations
	ofile << totalAlive << endl; // Writing number of alive cells

	// Writing coordinates of alive cells

	for (int i = 0; i < 100; i++) { // To control the index of second array
		x = secArray[i][0];
		y = secArray[i][1];

		ofile << x << " " << y << endl; // Displaying x and y coordinates after separation

		if (i == lastOfsec) {
			break;
		}
	}

	ofile.close(); // Closing the file
}