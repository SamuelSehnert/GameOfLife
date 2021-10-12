/*
	Author: Samuel Sehnert
	Date: 10/9/2021: October 9th, 2021
	Goal: To simulate Conway's Game of Life


   TODO:
      1) add a flag -f that will save the output to a file called "cell_history.out"
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/ioctl.h>
#include <unistd.h>

#define CELL_ALIVE 'X'
#define CELL_DEAD  '-'
#define DELTA_TURN_TIME 75000

#define ESC 27
#define ENTER 10

void setArrIndexFromCoord(int * cells, int gridSize, int x, int y);
int coordToIndex(int x, int y, int gridSize);

int randomCellData(int * cells, int gridSize);

void printCellData(int * cells, int gridSize);

void calculateTurn(int * cells, int gridSize);

int getCellNeighbors(int * cells, int cellIndex, int gridSize);

//void saveToFile(int * cells, int gridSize);

int main(int argc, char * argv[]){
	int * cells;
	int gridSize;
   unsigned long seed = time(NULL);
	int valid = 1;
	//int saveToFile = 0;

	if (argc != 2 && argc != 3){
		printf("\nUsage: gameOfLife <grid size n> <OPTIONAL: random seed>\n");
		return 1;
	}

	gridSize = atoi(argv[1]);
	if (gridSize > 255){
		printf("\nGrid too big!\n");
		return 1;
	}

   if (argc == 2){
      printf("\nSeed not entered. Will be set to: %ld\n", seed);
   }
   else if (argc == 3){
      seed = (unsigned long) atoi(argv[2]);
      printf("\nSeed entered as: %ld\n", seed);
   }

	cells = malloc(sizeof(int) * gridSize * gridSize);

	srand(seed);
	valid = randomCellData(cells, gridSize);

   printCellData(cells, gridSize);
   sleep(1);

	if (valid == -1){
		printf("Cells can only be placed as high as the grid size - 1!\n");
	}
	else{
		int turn = 0;
      while (1){
         usleep(DELTA_TURN_TIME);
			calculateTurn(cells, gridSize);
			printf("Turn: %d           Seed: %ld         Grid: %d x %d\n", turn, seed, gridSize, gridSize);
			printCellData(cells, gridSize);
			turn++;
		}
	}
	free(cells);
	return 0;
}

void setArrIndexFromCoord(int * cells, int gridSize, int x, int y){
	int index = (y * gridSize) + x;
	cells[index] = (int)CELL_ALIVE;
}

int coordToIndex(int x, int y, int gridSize){
	if (x < 0 || y < 0 || x >= gridSize || y >= gridSize){
		return -1;
	}
	return (gridSize * y) + x;
}

int checkIfCellValid(int cellIndex, int gridSize){
	if (cellIndex < 0 || cellIndex >= gridSize * gridSize){
		return -1;
	}
	return 1;
}

int checkIfCellAlive(int * cells, int cellIndex){
	if (cells[cellIndex] == CELL_ALIVE){
		return 1;
	}
	else{
		return 0;
	}
}

int randomCellData(int * cells, int gridSize){
	int cellCount = rand() % (gridSize * gridSize);
	int cellIndex;

	while (cellCount > 0){
		cellIndex = rand() % (gridSize * gridSize);
		if (checkIfCellValid(cellIndex, gridSize) != -1
		 		&& 
			checkIfCellAlive(cells, cellIndex) != 1){
				cells[cellIndex] = CELL_ALIVE;
		}
		cellCount--;
	}
	for (int i = 0; i < gridSize * gridSize; i++){
		if (cells[i] != CELL_ALIVE){
			cells[i] = CELL_DEAD;
		}
	}
	return 1;
}

void printCellData(int * cells, int gridSize){
	for (int i = 0; i < gridSize * gridSize; i++){
      if (cells[i] == CELL_ALIVE){
         printf("\033[96m");
		   printf("%c ", cells[i]);
         printf("\033[0m");
      }
      else{
         printf("\033[33m");
		   printf("%c ", cells[i]);
         printf("\033[0m");
      }
		if ((i + 1) % gridSize == 0 && i != 0){
			putchar('\n');
		}
	}
	putchar('\n');
}

//void saveToFile(int * cells, int gridSize){
//   for (int i = 0; i < gridSize * gridSize; i++){
//      //PLACE
//      if ((i + 1) % gridSize == 0 && i != 0){
//         //place '\n'
//      }
//   }
//}

int getCellNeighbors(int * cells, int cellIndex, int gridSize){
	int cellX, cellY;
	int currentIndex;
	int aliveNeighbors = 0;

	cellX = cellIndex % gridSize;
	cellY = cellIndex / gridSize;

	// up left
	currentIndex = coordToIndex(cellX-1, cellY-1, gridSize);
	if (checkIfCellValid(currentIndex, gridSize) != -1){
		aliveNeighbors += checkIfCellAlive(cells, currentIndex);
	}
	// up
	currentIndex = coordToIndex(cellX, cellY-1, gridSize);
	if (checkIfCellValid(currentIndex, gridSize) != -1){
		aliveNeighbors += checkIfCellAlive(cells, currentIndex);
	}
	// up right
	currentIndex = coordToIndex(cellX+1, cellY-1, gridSize);
	if (checkIfCellValid(currentIndex, gridSize) != -1){
		aliveNeighbors += checkIfCellAlive(cells, currentIndex);
	}
	// right
	currentIndex = coordToIndex(cellX+1, cellY, gridSize);
	if (checkIfCellValid(currentIndex, gridSize) != -1){
		aliveNeighbors += checkIfCellAlive(cells, currentIndex);
	}
	// down right
	currentIndex = coordToIndex(cellX+1, cellY+1, gridSize);
	if (checkIfCellValid(currentIndex, gridSize) != -1){
		aliveNeighbors += checkIfCellAlive(cells, currentIndex);
	}
	//down
	currentIndex = coordToIndex(cellX, cellY+1, gridSize);
	if (checkIfCellValid(currentIndex, gridSize) != -1){
		aliveNeighbors += checkIfCellAlive(cells, currentIndex);
	}
	//down left
	currentIndex = coordToIndex(cellX-1, cellY+1, gridSize);
	if (checkIfCellValid(currentIndex, gridSize) != -1){
		aliveNeighbors += checkIfCellAlive(cells, currentIndex);
	}
	//left
	currentIndex = coordToIndex(cellX-1, cellY, gridSize);
	if (checkIfCellValid(currentIndex, gridSize) != -1){
		aliveNeighbors += checkIfCellAlive(cells, currentIndex);
	}
	return aliveNeighbors;
}

void calculateTurn(int * cells, int gridSize){
	int newCells[gridSize * gridSize];

	for (int i = 0; i < gridSize * gridSize; i ++){
		int cell = cells[i];

		int aliveNeighbors = getCellNeighbors(cells, i, gridSize);
		if (cell == CELL_ALIVE){
			if (aliveNeighbors < 2){ //death by underpopulation
				newCells[i] = CELL_DEAD;

			}
			else if (aliveNeighbors == 2 || aliveNeighbors == 3){ //continues to live
				newCells[i] = CELL_ALIVE;
			}
			else if (aliveNeighbors > 3){//death by overpopulation
				newCells[i] = CELL_DEAD;
			}
		}
		else{
			if (aliveNeighbors == 3){ //make cell alive!!!
				newCells[i] = CELL_ALIVE;
			}
			else{
				newCells[i] = CELL_DEAD;
			}
		}
	}
	for (int i = 0; i < gridSize * gridSize; i++){
		cells[i] = newCells[i];
	}
}
