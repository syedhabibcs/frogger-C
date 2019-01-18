#include "log.h"


void mallocErrorChecker(void * retVal)
{
	if ( retVal == NULL ) 
	{
		fprintf(stderr, "Couldn't allocate memory, malloc failed.\n");
		exit(EXIT_FAILURE);
	}
}


void append(pthread_t newID, Log * newLog)
{
    struct node *temp,*right;
    temp= (struct node *)malloc(sizeof(struct node));
	mallocErrorChecker(temp);
    temp->id = newID;
	temp->data = newLog;
    right = (struct node *)head;

    if(right!=NULL)
	{
		while(right->next != NULL)
			right=right->next;
		right->next =temp;
		right=temp;
		right->next=NULL;
	}
	else
	{
		head = temp;
		head->next = NULL;
	}
}



void* frogLogInteraction()
{

	while(gameRunning)
	{

		pthreadMutexLock(&movementDetected_mutex);
		pthreadCondWait(&movementDetected_cv, &movementDetected_mutex);

		pthreadMutexLock(&linkList_mutex);

		
		if(gameRunning)
		{				
			frogOnLog = 0;

			if(frogRow > 0 && frogRow < 5)//if not on the land
			{		
				struct node *curr;
				curr=(struct node *)head;
				
				while(curr != NULL && frogOnLog==0)
				{
					Log log= *(curr->data);
					
					if(log.deleted == 0)//log is not already deleted
					{
						if(frogRow == log.rowNumber)//same log number
						{
							pthreadMutexLock(&frog_mutex);
							if(frogPosition[1] >= log.y && frogPosition[1] < log.y+23)//on log
								frogOnLog = 1;
							pthreadMutexUnlock(&frog_mutex);
						}
					}
					curr = curr->next;
				}
				if(frogOnLog==0) //decrease the lifes
				{	
					pthreadMutexUnlock(&linkList_mutex);//to avoid the deadlock
					pthreadMutexLock(&wonLost_mutex);
					pthreadCondSignal(&wonLost_cv);//if won
					pthreadMutexUnlock(&wonLost_mutex);
					pthreadMutexLock(&linkList_mutex);
				}
			}
		}			
		pthreadMutexUnlock(&linkList_mutex);
		pthreadMutexUnlock(&movementDetected_mutex);		
	}
	pthread_exit(NULL);
}







//-------------------------------------------------------------//

void* individualLog(void * parameters)
{
	int * param = ((int *) parameters);	

	
	Log * logPtr = malloc (sizeof (Log));;
	mallocErrorChecker(logPtr);
	
	logPtr->speed = param[0];
	logPtr->x = param[1];
	logPtr->y= param[2];
	logPtr->sleep = param[3];
	logPtr->rowNumber = param[4];
	logPtr->deleted = 0;
	
	int frogOnThisLog = 0;
	
	pthreadMutexLock(&linkList_mutex);
	append(pthread_self(),logPtr);	
	pthreadMutexUnlock(&linkList_mutex);
		
	
	
	while(gameRunning && logPtr->y<=GAME_COLS && logPtr->y>=-24)
	{
		for(int j=0; j<LOG_ANIM_TILES; j++)
		{
			for(int i=0; i< 3; i++)
			{				
				if(frogRow == logPtr->rowNumber)
				{
					if(logPtr->speed>0)
					{
						pthreadMutexLock(&frog_mutex);
						if(frogPosition[1]+1 >= logPtr->y && frogPosition[1] < logPtr->y+22)
						{
							frogOnLog = 1;
							frogOnThisLog = 1;
							if(frogPosition[1] <= GAME_COLS -3)
								frogPosition[1]++;
						}//on log
						pthreadMutexUnlock(&frog_mutex);
					}
					else
					{
						pthreadMutexLock(&frog_mutex);
						if(frogPosition[1]+1 >= logPtr->y && frogPosition[1] <= logPtr->y+23)
						{
							frogOnLog = 1;
							frogOnThisLog = 1;
							if(frogPosition[1]-1 >= 0)
								frogPosition[1]--;
						}//on log
						pthreadMutexUnlock(&frog_mutex);
					}					
				}
				
				pthreadMutexLock(&console_mutex);
				consoleClearImage(logPtr->x,logPtr->y - logPtr->speed,LOG_HEIGHT, strlen(LOG_GRAPHIC[0][1]));
				consoleDrawImage(logPtr->x, logPtr->y, LOG_GRAPHIC[j], LOG_HEIGHT);	//draw log
				if(frogOnThisLog)
				{
					pthreadMutexLock(&frog_mutex);
					consoleDrawImage(frogPosition[0], frogPosition[1], PLAYER_GRAPHIC[frogCurrentTile], PLAYER_HEIGHT);//rows,col
					pthreadMutexUnlock(&frog_mutex);
				}
 
				pthreadMutexUnlock(&console_mutex);
				sleepTicks(logPtr->sleep);
				logPtr->y+= logPtr->speed;
			}
		}
	}
	
	if(frogOnThisLog==1)//log on which the frog was died
	{
		frogOnLog=0;
		pthreadMutexLock(&movementDetected_mutex);
		pthreadCondSignal(&movementDetected_cv);
		pthreadMutexUnlock(&movementDetected_mutex);	
	}
	logPtr->deleted = 1;
		
	pthread_exit(NULL);
}

//-------------------------------------------------------------//


void* spawnLogRow(void * parameters)
{
	//todo
	//clean thread as they go out

	pthread_t logs;

	while(gameRunning)
	{
		pthreadCreate(&logs, NULL, individualLog, parameters);

		int randomNum = (rand() % 150 + (-50)) + 250;
		//sleepTicks(randomNum);
		for (int j=1; j < randomNum && gameRunning; j+=10){
			sleepTicks(j);
			randomNum -= j;
		}

	}
			
	free(parameters);
	
	pthread_exit(NULL);
}

