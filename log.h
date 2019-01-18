/*
 Purpose: draws logs and takes care of interaction with frog
*/

#ifndef LOG_H
#define LOG_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameglobals.h"
#include "threadwrappers.h"
#include "frogger.h"


#define LOG_ANIM_TILES 5
#define LOG_HEIGHT 4
static char* LOG_GRAPHIC[LOG_ANIM_TILES][LOG_HEIGHT] = {
   {"/======================\\",
    "|    x                 |",
    "|       x              |",
   "\\======================/"},
   
   {"/-=====================\\",
    "|       x              +",
    "|     x                |",
   "\\======================/"},
   
   {"/======================\\",
    "-           x          |",
    "|             x        +",
   "\\======================/"},
  
  {"/======================\\",
   "|             x        +",
   "-           x          |",
   "\\======================/"},
  
  {"/======================\\",
   "|               x      |",
   "|                 x    +",
   "-======================/"}
};

void* individualLog();
void* spawnLogRow();
void* frogLogInteraction();
void mallocErrorChecker(void * retVal);
#endif
