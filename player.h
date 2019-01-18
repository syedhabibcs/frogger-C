/*
 Purpose: draws frog repeately (animating frog) and controlling it.
*/


#ifndef PLAYER_H
#define PLAYER_H

#include <pthread.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"

#include "gameglobals.h"
#include "threadwrappers.h"
#include "frogger.h"

#define PLAYER_ANIM_TILES 12
#define PLAYER_HEIGHT 2

static char* PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};


void* animatePlayer();
void* controlFrog();
#endif
