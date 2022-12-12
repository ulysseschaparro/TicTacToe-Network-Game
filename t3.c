// Tic-tac-toe (T3) Network Game
// Jason Losh origin code

//-----------------------------------------------------------------------------
// Compile notes for C99 code
//-----------------------------------------------------------------------------

// gcc -o t3 t3.c udp.c

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdlib.h>          // EXIT_ codes
#include <stdbool.h>         // bool
#include <stdio.h>           // printf, scanf
#include <string.h>          // strlen, strcmp

#include "udp.h"

#define CLIENT_PORT 4096
#define SERVER_PORT 4097

void clearBoard(char board[3][3])
{
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            board[r][c] = '.';
}

// TODO: Add code to display the game board
void showBoard(char board[3][3])
{
  printf("\n\t%c\t%c\t%c",board[0][0],board[0][1],board[0][2]);
  printf("\n\t%c\t%c\t%c",board[1][0],board[1][1],board[1][2]);
  printf("\n\t%c\t%c\t%c\n",board[2][0],board[2][1],board[2][2]);
}

// TODO: Add code to determine if a winner (xo) has occurred
bool isWinner(char board[3][3], char xo)
{
    bool win = false;
    int count = 0;
    //horizontal
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if(board[r][c] != xo) break;
            count++;
            if(count == 3) return !win;
        }
        count = 0;
    }
    //vertical
    for (int c = 0; c < 3; c++) {
        for (int r = 0; r < 3; r++) {
            if(board[r][c] != xo) break;
            count++;
            if(count == 3) return !win;
        }
        count = 0;
    }
    //diagonal (\)
      for (int d = 0; d < 3; d++) {
              if(board[d][d] != xo) break;
              count++;
              if(count == 3) return !win;
      }
      count = 0;
    //diagonal (/)
    int c = 0;
    for (int r = 3; r > 0; r--) {
      if(board[r-1][count] != xo) break;
      count++;
      if(count == 3) return !win;
    }
    count = 0;

    return win;
}

// TODO: Add code to test if an x or o (xo) is a valid move
//       and then record in the game board
bool addMove(char board[3][3], char move[], char xo)
{
    int r = 0;
    int c = 0;
	
    bool length_ok = false;
    if(strlen(move) == 2) {
       length_ok = true; 
    }
    if(!length_ok) {
	return false;
    }

    bool first_ok = false;
    if(move[0] == 'A') {
        first_ok = true;
        r = 0;
    }
    if(move[0] == 'B') {
        first_ok = true;
        r = 1;
    }
    if(move[0] == 'C') {
        first_ok = true;
        r = 2;
    }
    if(!first_ok) {
      return false;
    }

    bool second_ok = false;
    if(move[1] == '1') {
        second_ok = true;
        c = 0;
    }
    if(move[1] == '2') {
        second_ok = true;
        c = 1;
    }
    if(move[1] == '3') {
        second_ok = true;
        c = 2;
    }
    if(!second_ok) {
      return false;
    }

    bool legal = false;
    if(board[r][c] == '.') {
	board[r][c] = xo;
	legal = true;
    }
    if(!legal) {
	printf("This position has been played. Try again...\n");
    }

    return legal;
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    char *remoteIp;
    char *role;
    char str[100]; 		
    char board[3][3];
    char move[100], move2[100];
    bool validMove;
    bool winner = false;
    int moveCount = 0;

    bool myTurn;
    char myLetter, opponentLetter;
    int remotePort;

    // (DONE) TODO : Verify arguments are valid
    bool goodArguments = true;
    if(argc == 3) {
	remoteIp = argv[1];
	if(strcmp(argv[2], "invite") != 0 && strcmp(argv[2], "accept") != 0) {
		goodArguments = false;
	}
	else role = argv[2];
	printf("%s\n", role); 
    }
    else goodArguments = false;
    if (!goodArguments)
    {
        printf("usage: t3 IPV4_ADDRESS ROLE\n");
        printf("  where:\n");
        printf("  IPV4_ADDRESS is address of the remote machine\n");
        printf("  ROLE is either invite or accept\n");
        exit(EXIT_FAILURE);
    }

    // (DONE) TODO: Determine if client or server
    // A server will wait to accept an invitation to play
    // A client will send an invitation to play
    bool client;
    
    if(strcmp(role, "accept") == 0) {
	client = false;
	remotePort = CLIENT_PORT;
    }
    else {
	client = true;
	remotePort = SERVER_PORT;
    }


    // (DONE) TODO: Open listener port number dependent on client/server role
    bool opened = false;
    if(!client) {
	opened = openListenerPort(remoteIp, SERVER_PORT);
    }
    else {
	opened = openListenerPort(remoteIp, CLIENT_PORT);
    }
    
    if(!opened) {
	printf("Listener port failed to open.\n");
	exit(EXIT_FAILURE);
    }

	printf("wyd here\n");
    // (DONE) TODO: Determine remote port that you will send data to
    //       If you are server, send to client port, and vice versa

    // Setup game
    clearBoard(board);

    // (DONE) TODO: Determine whether it is your turn or not
    if(client) myTurn = false;
    else myTurn = true;

    // (DONE) TODO: Determine your letter (x or o) and your opponent's letter

    // (DONE) TODO: Add code to send an invite or wait to accept an invite
    bool sent = false; 

    if(!client) {
	printf("waiting to receive\n");
        receiveData(str, 100); //not sure
	printf("received data\n"); 
	if(strcmp(str, "invite") != 0) {  //not sure
		printf("Did not receive an invitation. Exiting...\n");
		exit(EXIT_FAILURE);
	}
	myLetter = 'X'; opponentLetter = 'O';
    }
    
    else {
 	sent = sendData(remoteIp, remotePort, "invite");
	if(sent) {
		printf("Invitation has been sent.\n");
		myLetter = 'O'; opponentLetter = 'X';
	}
	else {
		printf("Invitation failed to send.\n");
		exit(EXIT_FAILURE);
	}
    }


    // Start game loop, alternating turns
    while(!winner && moveCount != 9)
    {
        // get my move
        if (myTurn)
        {
            // (DONE) TODO: add code your move here to get the move, validate move,
            //       show board, send move to remote port, and check for a winner
	  
      	  while(true) {
		printf("Enter your move (%c): ", myLetter);
          	scanf("%s", move);
        	validMove = addMove(board, move, myLetter);
       		if(validMove) {
       		   break;
       	  	} 
          }
      		showBoard(board);
      		sent = sendData(remoteIp, remotePort, move);
      		if(!sent) {
      			printf("Move failed to send. Exiting...\n");
      			exit(EXIT_FAILURE);
      		}
      		winner = isWinner(board, myLetter);
          if(winner) {
            printf("\nYou (%c) won\n",myLetter);
            break;
          }
        }
        // get opponent's move
        else
        {
            // (DONE) TODO: add code to receive your opponent's move, validate move,
            //       show board, and check for a winner
      		receiveData(move2, 100);
      		validMove = addMove(board, move2, opponentLetter);
      		if(!validMove) {
      			printf("Invalid move received. Exiting...\n");
      			exit(EXIT_FAILURE);
      		}
          printf("\nYour opponent (%c) moved to %s\n", opponentLetter, move2);
      		showBoard(board);
      		winner = isWinner(board, opponentLetter);
          if(winner) {
            printf("\nYour opponent (%c) won\n",opponentLetter);
            break;
          }
        }
        // Increment move count, alternate turns
        moveCount++;
        myTurn = !myTurn;
    }
    if (!winner)
        printf("The game was a draw\n");


    // (DONE) TODO: Close listener port
    closeListenerPort();
    return EXIT_SUCCESS;
}
