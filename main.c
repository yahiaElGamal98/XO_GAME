#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable : 4996)   //directive to be able to use scanf in visual studio


#define ROWS 3   //define rows and columns as 3x3 matrix
#define COLUMNS 3
#define Continue_Playing 0   //some flags to check game condition
#define WON 1
#define OCCUPIED 1

typedef unsigned char uint8_t;   //no more than 8 bits are needed to do the functionality of this code

typedef struct XO
{
	uint8_t player:7;   //player X or Y
	uint8_t place_Occupied:1;   //place occupied Flag 1 if occupied 0, if empty      
}XO;


void initPlayers(uint8_t players[],uint8_t sizeofPlayers);   //function to initialize players playing sequence
void tutorial(void);   //print game tutorial function
void checkValidity(uint8_t* place,XO xoGame[ROWS][COLUMNS]);   //func. to check the validity of X/O place either it is taken or out of field
void play(uint8_t players[], uint8_t sizeofPlayers,XO xoGame[ROWS][COLUMNS] );   //the game function
uint8_t checkStatus(XO xoGame[ROWS][COLUMNS]);   //func. to check winning condition
void printGame(XO xoGame[ROWS][COLUMNS]);   //func. to print game



int main(void)
{
	XO xoGame[ROWS][COLUMNS] = { {{'\0',0},{'\0',0},{'\0',0}},   //initializing gameboard with nulls and all places are available
							 {{'\0',0},{'\0',0},{'\0',0}},
							 {{'\0',0},{'\0',0},{'\0',0}} };
	uint8_t players[ROWS*COLUMNS];   //initialitizing playing sequence array of size rows*columns in max case
	tutorial();   //printing tutorial of game
	initPlayers(players, ROWS*COLUMNS);   //initializing playing sequence depending on the starting player
	play(players,ROWS*COLUMNS,xoGame);   //playing the game.
	system("PAUSE");
	return 0;
}

void tutorial(void)
{
	printf("The places are numbered 0 through 8 as follows:\n");
	printf("-----------\n");
	printf(" %d | %d | %d \n", 0, 1, 2);   //printing the numbers needed by the user to place his X or O in the desired position
	printf("---|---|---\n");
	printf(" %d | %d | %d \n", 3, 4, 5);
	printf("---|---|---\n");
	printf(" %d | %d | %d \n", 6, 7, 8);
	printf("-----------\n");
	printf("Please only use the numbers above to place your X/O\n");
	printf("Empty places will have their number inside them while you play\n\n\n");
}

void initPlayers(uint8_t players[], uint8_t size)
{
	uint8_t validityFlag = 0;   //valid input flag
	uint8_t temp;   //temp variable to store the character till we check validity
	printf("Player 1 Please Enter either X or O :");
	while (!validityFlag)
	{
		scanf("%c", &temp);
		getchar();   //eating up the Enter '\n' character entered by user
		if (!(('X' == temp) || ('O' == temp)))   //only 'X' or 'O' allowed
		{
			printf("Invalid playing character. Please enter either 'X' or 'O' case sensitive:");	
		}
		else 
			validityFlag = 1;   //input valid, break out the loop
	}

	if ('X' == temp)   // if player 1 will play with X
	{
		int i;
		for (i = 0; i < size; i += 2)   //increment with two
		{
			players[i] = 'X';   //first element will be X
			if ((i + 1) <= (size-1))   //protection not to access element 9 by mistake where the max is 8 (size-1)
			{
				players[i + 1] = 'O';   //the element after it will be O and vice versa in else if below
			}
		}
	}
	else if ('O' == temp)   
	{
		int i;
		for (i = 0; i < (ROWS*COLUMNS); i += 2)
		{
			players[i] = 'O';
			if ((i + 1) <= (size-1))
			{
				players[i + 1] = 'X';
			}
		}
	}
}

void checkValidity(uint8_t* place,XO xoGame[ROWS][COLUMNS])
{
	uint8_t validityFlag = 0,temp;   //checking validity of position entered by user
	while (!validityFlag)   //while not valid
	{
		scanf("%c", &temp);   //entering the place value as a character to prevent crashing
		getchar();   //eating the new line entered by the user
		temp = temp - '0';   //changing the value from ascii to integer value to use it in indexing
		if ((temp) > 8 || ((temp) < 0))
		{
			printf("Invalid position, please only enter positions as tutorial suggested.\n");
		}
		else if ((xoGame[(temp) / ROWS][(temp) % COLUMNS].place_Occupied) == 1)   // checking if place is occupied by another user
		{
			printf("Place taken, please choose another place.\n");
		}
		else
		{
			validityFlag = 1;   //place is valid
			*place = temp;   //return the value
		}
	}
}
void play(uint8_t players[], uint8_t sizeofPlayers, XO xoGame[ROWS][COLUMNS])
{
	uint8_t place;
	uint8_t numTurn;   //number of turns, it can reach a maximum of 9 turns.
	for (numTurn = 0; numTurn < ROWS*COLUMNS; numTurn++)
	{
		printf("Player '%c' Enter desired position: \n", players[numTurn]);
		checkValidity(&place,xoGame);
		xoGame[place / ROWS][place % COLUMNS].place_Occupied = OCCUPIED;   //raise the occupy flag
		xoGame[place / ROWS][place % COLUMNS].player = players[numTurn];   //define the player occupying the place
		printGame(xoGame);   //print the game for the user
		if (WON == checkStatus(xoGame))
		{
			break;   //stop the game if one of the players win
		}
		else if(numTurn==ROWS*COLUMNS-1) //max number of plays =8
		{
			printf("DRAW!\n");   //print draw if no one won and we reached max plays
			break;
		}
		else if (Continue_Playing == checkStatus(xoGame))   //the game is still being played
		{
			continue;
		}
	}
}

void printGame(XO xoGame[ROWS][COLUMNS])   
{
	printf("-----------\n");
	uint8_t rowIndex;   //current row index value
	uint8_t columnIndex;   // current column indexValue
	uint8_t emptyPlaceIndicator = 0;   //used to fill unused places with corresponding numbers
	for (rowIndex = 0; rowIndex < ROWS; rowIndex++)
	{
		for (columnIndex = 0; columnIndex < COLUMNS; columnIndex++)
		{
			if (OCCUPIED == (xoGame[rowIndex][columnIndex].place_Occupied)) 
			{
				printf(" %c |", xoGame[rowIndex][columnIndex].player);   //print the occupied place's 'X' or 'Y'
			}
			else
			{
				printf("'%d'|", emptyPlaceIndicator);   // print number of place if not occupied
			}
			emptyPlaceIndicator++;   //increment the number to start from 0 to 8 like the places' values
		}
		printf("\n");
		printf("---|---|---\n");
	}
}
uint8_t checkStatus(XO xoGame[ROWS][COLUMNS])
{
	uint8_t i;   //Index value
	for (i = 0; i < ROWS; i++)   // check if any of the columns match ('Vertical matching win condition')
	{
		if ((xoGame[i][0].player == xoGame[i][1].player) && (xoGame[i][1].player == xoGame[i][2].player) && (xoGame[i][1].player!='\0'))
		{
			printf("Player %c WON!\n",xoGame[i][0].player);
			return WON;
		}
		else
		{
			continue;
		}
	}
	for (i = 0; i < COLUMNS; i++)   // check if any of the rows match ('Horizontal matching win condition')
	{
		if ((xoGame[0][i].player == xoGame[1][i].player) && (xoGame[1][i].player == xoGame[2][i].player)&& (xoGame[2][i].player!='\0'))
		{
			printf("Player %c WON!\n", xoGame[0][i].player);
			return WON;
		}
		else
		{
			continue;
		}
	}
	if ((xoGame[0][0].player == xoGame[1][1].player) && (xoGame[1][1].player == xoGame[2][2].player) && (xoGame[2][2].player!='\0'))
	{
		printf("Player %c WON!\n", xoGame[0][0].player);   // check for first diagonal winnig condition
		return WON;
	}
	else if ((xoGame[0][2].player == xoGame[1][1].player) && (xoGame[1][1].player == xoGame[2][0].player) && (xoGame[2][0].player != '\0'))
	{
		printf("Player %c WON!\n", xoGame[0][2].player);   // check for second diagonal winnig condition
		return WON;
	}
	else 
	{
		return Continue_Playing;   //game is still underway, continue
	}

}