#include "frogger.h"


/**** DIMENSIONS MUST MATCH the ROWS/COLS */

char *GAME_BOARD[] = {
"                                   Lives:",
"/------\\          /------\\          /------\\          /------\\          /------\\",
"|      |          |      |          |      |          |      |          |      |",
"+      +----------+      +----------+      +----------+      +----------+      +",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"",
"",
"",
"" };



//-------------------------------------------------------------//


void runGame()
{
	if (consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD))
	{		

		initializeVariables();
		
		pthread_t animateFrog;
		pthread_t screenRefresh;
		pthread_t keyboard;
		pthread_t spawnLogRowsThread[4];
		pthread_t frogLogInteractionThread;
		pthread_t upKeepThread;

		
		//startup all the threads				
		pthreadCreate(&frogLogInteractionThread, NULL, frogLogInteraction, NULL);
		pthreadCreate(&animateFrog, NULL, animatePlayer, NULL);
		pthreadCreate(&screenRefresh, NULL, refreshScreen, NULL);
		pthreadCreate(&keyboard, NULL, controlFrog, NULL);
	    		
	
		//first log row
		int * parameters = (int*)malloc((5)*sizeof(int));
		mallocErrorChecker(parameters);
		parameters[0] = 1;//speed
		parameters[1] = 16;//x
		parameters[2] = -24;//y
		parameters[3] = 5;//sleep ticks
		parameters[4] = 1;//rowNumber

		pthreadCreate(&spawnLogRowsThread[0], NULL, spawnLogRow, parameters);


		//second log row
		parameters = (int*)malloc((5)*sizeof(int));
		mallocErrorChecker(parameters);
		parameters[0] = -1;//speed
		parameters[1] = 12;//x
		parameters[2] = GAME_COLS;//y
		parameters[3] = 4;//sleep ticks
		parameters[4] = 2;//rowNumber
		pthreadCreate(&spawnLogRowsThread[1], NULL, spawnLogRow, parameters);
		
		
		//third log row
		parameters = (int*)malloc((5)*sizeof(int));	
		mallocErrorChecker(parameters);
		parameters[0] = 1;//speed
		parameters[1] = 8;//x
		parameters[2] = -24;//y
		parameters[3] = 3;//sleep ticks
		parameters[4] = 3;//rowNumber
		pthreadCreate(&spawnLogRowsThread[2], NULL, spawnLogRow, parameters);
		
		
		//fourth log row
		parameters = (int*)malloc((5)*sizeof(int));	
		mallocErrorChecker(parameters);
		parameters[0] = -1;//speed
		parameters[1] = 4;//x
		parameters[2] = GAME_COLS;//y
		parameters[3] = 2;//sleep ticks
		parameters[4] = 4;//rowNumber		
		pthreadCreate(&spawnLogRowsThread[3], NULL, spawnLogRow, parameters);

		pthreadCreate(&upKeepThread, NULL, upKeep, NULL);
		
		
		
		//-------------------------------------------------------------//

	
		//go to sleep until game finishes
		pthreadMutexLock(&gameEnded_mutex);
		pthreadCondWait(&gameEnded_cv, &gameEnded_mutex);
		
		gameRunning = 0;
		pthreadMutexUnlock(&gameEnded_mutex);
		
	
		//game finished clean up stuff
		pthreadMutexLock(&movementDetected_mutex);
		pthreadCondSignal(&movementDetected_cv);
		pthreadMutexUnlock(&movementDetected_mutex);

		pthreadMutexLock(&wonLost_mutex);
		pthreadCondSignal(&wonLost_cv);
		pthreadMutexUnlock(&wonLost_mutex);

		
		/* wait for final key before killing curses and game */
		pthreadMutexLock(&console_mutex);
		finalKeypress(); 
		pthreadMutexUnlock(&console_mutex);

		
		//-----------Join all the threads
		pthreadJoin(frogLogInteractionThread);
		pthreadJoin(animateFrog);		
		pthreadJoin(screenRefresh);
		pthreadJoin(keyboard);
		pthreadJoin(spawnLogRowsThread[0]);
		pthreadJoin(spawnLogRowsThread[1]);
		pthreadJoin(spawnLogRowsThread[2]);
		pthreadJoin(spawnLogRowsThread[3]); 
		pthreadJoin(upKeepThread);

		cleanStuff();

        }       
        consoleFinish();        		
}



//-------------------------------------------------------------//

void initializeVariables()
{
		frogRow = 0;
		frogOnLog = 0;
		lives = 4;		
		gameRunning = 1;
		
		
		//initialize mutex variables
		pthreadMutexInit(&frog_mutex);
		pthreadMutexInit(&console_mutex);
		pthreadMutexInit(&linkList_mutex);
		pthreadMutexInit(&wonLost_mutex);
		pthreadMutexInit(&gameEnded_mutex);
		pthreadMutexInit(&movementDetected_mutex);
		
		//initialize condition variables
		pthreadCondInit(&wonLost_cv);
		pthreadCondInit(&gameEnded_cv);	
		pthreadCondInit(&movementDetected_cv);
		
		//initialize frog position
		prevFrogPosition[0] = 21;
		prevFrogPosition[1] = 40;
		frogPosition[0] = 21;
		frogPosition[1] = 40;
		frogCurrentTile = 0;

		winningSpots[0]=0;
		winningSpots[1]=0;
		winningSpots[2]=0;
		winningSpots[3]=0;		
		winningSpots[4]=0;
}

void cleanStuff()
{		
		//-----------clean mutex and condition variables
		pthreadCondDestroy(&wonLost_cv);
		pthreadCondDestroy(&gameEnded_cv);
		pthreadCondDestroy(&movementDetected_cv);		
		
		pthreadMutexDestroy(&movementDetected_mutex);
		pthreadMutexDestroy(&gameEnded_mutex);
		pthreadMutexDestroy(&wonLost_mutex);
		pthreadMutexDestroy(&linkList_mutex);
		pthreadMutexDestroy(&console_mutex);
		pthreadMutexDestroy(&frog_mutex);	
}





//-------------------------------------------------------------//

void* refreshScreen()
{
	while(gameRunning)
	{
		pthreadMutexLock(&console_mutex);
		consoleRefresh();
		pthreadMutexUnlock(&console_mutex);
		sleepTicks(1);
	}
	
	pthread_exit(NULL);
}





//-------------------------------------------------------------//



void* upKeep()
{
	
	char livesChar = '0' + lives;
	
	pthreadMutexLock(&console_mutex);
	putString(&livesChar, 0, 42, 1);
	pthreadMutexUnlock(&console_mutex);


	int numWins = 0;	
	
	//call this when someone dies or wins

	while(gameRunning && lives>0 && numWins<5)
	{
		//todo
		//check if someone died or won
		
	
		pthreadMutexLock(&wonLost_mutex);
		pthreadCondWait(&wonLost_cv, &wonLost_mutex);
	

		
		if(gameRunning)
		{
			sleepTicks(2);//for the frog appear on moved position
			

			if(frogRow != 5)//died
			{
				//decrease the lives
				//put a pause to game (take everything and go to sleep)
				//place the frog in right position
				
				lives--;
						
				if(lives<1)	//show lose message and call end game
				{			
					pthreadMutexLock(&console_mutex);
					putBanner("game over..Do, or do not.. there is no try!");
					disableConsole(1);
					pthreadMutexUnlock(&console_mutex);	
					
					pthreadMutexLock(&gameEnded_mutex);
					pthreadCondSignal(&gameEnded_cv);
					pthreadMutexUnlock(&gameEnded_mutex);	
				}

				else
				{

					pthreadMutexLock(&console_mutex);//console is first to avoid deadlock

					pthreadMutexLock(&frog_mutex);	
								
					livesChar = '0' + lives;
					putString(&livesChar, 0, 42, 1);

					
					frogRow = 0;
					frogOnLog = 0;
					
					sleepTicks(50);

					prevFrogPosition[0] = 21;
					prevFrogPosition[1] = 40;
					frogPosition[0] = 21;
					frogPosition[1] = 40;

					pthreadMutexUnlock(&frog_mutex);
					
					pthreadMutexUnlock(&console_mutex);

				}
			}

			else //if won
			{
				//assumption: in valid spot
				//put frog on starting position
				//put frog on winning position
				//check if all positions filled, if yes exit the game

				int k;
				numWins = 0;
				for(k=0; k<5; k++)
					numWins += winningSpots[k];
				

				if(numWins==5)
				{
					//show win message and call end game
					pthreadMutexLock(&console_mutex);
					putBanner("Holdup, someone actually won the game!");
					disableConsole(1);
					pthreadMutexUnlock(&console_mutex);	
					
					pthreadMutexLock(&gameEnded_mutex);
					pthreadCondSignal(&gameEnded_cv);
					pthreadMutexUnlock(&gameEnded_mutex);	
				}

				else
				{
					pthreadMutexLock(&frog_mutex);	
					
					frogRow = 0;
					frogOnLog = 0;
					
					prevFrogPosition[0] = 21;
					prevFrogPosition[1] = 40;
					frogPosition[0] = 21;
					frogPosition[1] = 40;

					pthreadMutexUnlock(&frog_mutex);
				}
			}
		}
		
		pthreadMutexLock(&linkList_mutex);
		
		//remove deleted logs
		struct node *prev;		
		struct node *curr;
		prev = NULL;
		curr=(struct node *)head;
			
		while(curr != NULL)
		{
			Log log= *(curr->data);//get the currentLog

			
			if(log.deleted == 1)//log is deleted
			{
				pthreadJoin(curr->id);//wait for the thread to exit
				free(curr->data);//free the logPtr
				
				//remove this node from Linklist
				if(prev == NULL)//remove the first node
					head = curr->next;
				else
					prev->next = curr->next;

				free(curr);//free the deleted node				

				if(prev == NULL)//curr to next pointer
					curr = head;
				else
					curr = prev->next;//the next node in list

				
				
			}
			else
			{
				prev = curr;
				curr = curr->next;
			}

		}

		pthreadMutexUnlock(&linkList_mutex);		
		pthreadMutexUnlock(&wonLost_mutex);
	}
		
	pthread_exit(NULL);
}




