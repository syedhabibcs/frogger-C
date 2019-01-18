/*
 Purpose: Contains the definitions of structs and global variables.
*/


#ifndef GAMEGLOBALS_h
#define GAMEGLOBALS_h

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


//data structures
struct singleLog{
	int x;
	int y;
	int speed;
	int sleep;
	int rowNumber;
	int deleted;
};

typedef struct singleLog Log;

struct node
{
    pthread_t id;
	Log * data;		
    struct node *next;
};


extern struct node *head;

//mutexes
extern pthread_mutex_t	frog_mutex;	
extern pthread_mutex_t	console_mutex;
extern pthread_mutex_t	linkList_mutex;

//condition variables
extern pthread_mutex_t wonLost_mutex;
extern pthread_cond_t wonLost_cv;

extern pthread_mutex_t	gameEnded_mutex;
extern pthread_cond_t  gameEnded_cv;

extern pthread_mutex_t movementDetected_mutex;
extern pthread_cond_t  movementDetected_cv;

//-------------------------------------------------------------//
//critical resources
extern int frogPosition [2];
extern int prevFrogPosition [2];
extern int frogCurrentTile;

extern int frogRow;
extern int frogOnLog;
extern int lives;
extern int winningSpots [5];

extern int gameRunning;


#endif
