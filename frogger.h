/*
 Purpose: Starts up all the threads and goes to sleep
*/

#ifndef FROGGER_H
#define FROGGER_H
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "console.h"
#include "threadwrappers.h"
#include "log.h"
#include "gameglobals.h"


#define GAME_ROWS 24
#define GAME_COLS 80



void initializeVariables();
void cleanStuff();

void runGame();
void* refreshScreen();


void* upKeep();

#endif
