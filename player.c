
#include "player.h"

//----------------------------------------------------//
//variables

//mutexes
pthread_mutex_t	frog_mutex;	
pthread_mutex_t	console_mutex;
pthread_mutex_t	linkList_mutex;

//condition variables
pthread_mutex_t wonLost_mutex;
pthread_cond_t wonLost_cv;

pthread_mutex_t	gameEnded_mutex;
pthread_cond_t  gameEnded_cv;

pthread_mutex_t movementDetected_mutex;
pthread_cond_t  movementDetected_cv;



//critical resources
int frogPosition [2];
int prevFrogPosition [2];
int frogCurrentTile;

int frogRow;
int frogOnLog;
int lives;
int winningSpots [5];

int gameRunning;

struct node *head;

//----------------------------------------------------//

void* animatePlayer()
{
	int i;
	int j = 15;
	while(gameRunning)
	{
		for (i = 0; i<j; i++)
		{
			pthreadMutexLock(&console_mutex);
			pthreadMutexLock(&frog_mutex);
			consoleClearImage(prevFrogPosition[0],prevFrogPosition[1],PLAYER_HEIGHT, strlen(PLAYER_GRAPHIC[frogCurrentTile][0]));
			consoleDrawImage(frogPosition[0], frogPosition[1], PLAYER_GRAPHIC[frogCurrentTile], PLAYER_HEIGHT);//rows,col
			prevFrogPosition[0] = frogPosition[0];
			prevFrogPosition[1] = frogPosition[1];			
			pthreadMutexUnlock(&frog_mutex);
			pthreadMutexUnlock(&console_mutex);
			sleepTicks(2);
		}
	
		frogCurrentTile++;
		if(frogCurrentTile==2)
			frogCurrentTile = 0;
			
	}
	
	pthread_exit(NULL);
}


//-------------------------------------------------------------//

void* controlFrog()
{
	char check;
	fd_set set;
	int ret = 0;
	struct timespec timeout;
	//get first char
    while(gameRunning && ret<=0)//exit loop if game ended or got input
    {
		FD_ZERO(&set);
		FD_SET(STDIN_FILENO, &set);
		timeout = getTimeout(1); /* duration of one tick */
		ret = pselect(FD_SETSIZE, &set, NULL, NULL, &timeout, NULL);
    }

	
	while(gameRunning)
	{
		ret = 0;
		check = getchar();

		switch (check)
	       {
			case 'd':
				pthreadMutexLock(&frog_mutex);
				if(frogPosition[1] <= GAME_COLS -3)
				{
					frogPosition[1]++;
					pthreadMutexLock(&movementDetected_mutex);
					pthreadCondSignal(&movementDetected_cv);
					pthreadMutexUnlock(&movementDetected_mutex);
				}
				pthreadMutexUnlock(&frog_mutex);
				break;
				
			case 'a':
				pthreadMutexLock(&frog_mutex);
				if(frogPosition[1]-1 >= 0)
				{
					frogPosition[1]--;
					pthreadMutexLock(&movementDetected_mutex);
					pthreadCondSignal(&movementDetected_cv);
					pthreadMutexUnlock(&movementDetected_mutex);
				}
				pthreadMutexUnlock(&frog_mutex);
				break;
				
			case 'w':
				pthreadMutexLock(&frog_mutex);
				if(frogPosition[0]-4 >= 0)
				{
					frogPosition[0]-=4;
					if(frogPosition[0] >= 20)
						frogRow = 0;
					else if(frogPosition[0] >= 16)
						frogRow = 1;
					else if(frogPosition[0] >= 12)
						frogRow = 2;
					else if(frogPosition[0] >= 8)
						frogRow = 3;
					else if(frogPosition[0] >= 4)
						frogRow = 4;
					else//if this check if in the right place
					{
						int won = 0;
						if( winningSpots[0]==0 && frogPosition[1]>0 && frogPosition[1] <6)
						{
							winningSpots[0] = 1;// close this spot
							won=1;
						}
						else if( winningSpots[1]==0 && frogPosition[1]>18 && frogPosition[1] <24)
						{
							winningSpots[1] = 1;// close this spot
							won=1;
						}
						else if( winningSpots[2]==0 && frogPosition[1]>36 && frogPosition[1] <42)
						{
							winningSpots[2] = 1;// close this spot
							won=1;
						}
						else if( winningSpots[3]==0 && frogPosition[1]>54 && frogPosition[1] <60)
						{
							winningSpots[3] = 1;// close this spot
							won=1;
						}
						else if( winningSpots[4]==0 && frogPosition[1]>72 && frogPosition[1] <78)
						{
							winningSpots[4] = 1;// close this spot
							won=1;
						}
						else
						{
							frogPosition[0]+=4;
						}//couldn't land on a valid spot
												
						if(won)
						{//tell upKeep to update the frog position and check if completed the game
							frogPosition[0]+=1;//remove one extra space
							frogRow = 5;
							pthreadMutexLock(&wonLost_mutex);
							pthreadCondSignal(&wonLost_cv);//if won
							pthreadMutexUnlock(&wonLost_mutex);
						}
						
					}
					pthreadMutexUnlock(&frog_mutex);					
					pthreadMutexLock(&movementDetected_mutex);
					pthreadCondSignal(&movementDetected_cv);
					pthreadMutexUnlock(&movementDetected_mutex);				}
				break;
				
			case 's':
				pthreadMutexLock(&frog_mutex);
				if(frogPosition[0]+4 <= GAME_ROWS)
				{	frogPosition[0]+=4;
					if(frogPosition[0] >= 20)
						frogRow = 0;
					else if(frogPosition[0] >= 16)
						frogRow = 1;
					else if(frogPosition[0] >= 12)
						frogRow = 2;
					else if(frogPosition[0] >= 8)
						frogRow = 3;
					else if(frogPosition[0] >= 4)
						frogRow = 4;

					pthreadMutexLock(&movementDetected_mutex);
					pthreadCondSignal(&movementDetected_cv);
					pthreadMutexUnlock(&movementDetected_mutex);				}
				pthreadMutexUnlock(&frog_mutex);
				break;
			case 'q': 
				pthreadMutexLock(&console_mutex);
				putBanner("quitter....");
				disableConsole(1);
				pthreadMutexUnlock(&console_mutex);
				
				pthreadMutexLock(&gameEnded_mutex);
				pthreadCondSignal(&gameEnded_cv);
				pthreadMutexUnlock(&gameEnded_mutex);			
				break;
	        }
		sleepTicks(3);
		
		while(gameRunning && ret<=0)
		{
			FD_ZERO(&set);
			FD_SET(STDIN_FILENO, &set);
			timeout = getTimeout(1); /* duration of one tick */
			ret = pselect(FD_SETSIZE, &set, NULL, NULL, &timeout, NULL);
		}//only continue if game ended or got input
	}

	pthread_exit(NULL);
}
