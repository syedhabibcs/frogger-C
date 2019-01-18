#include "threadwrappers.h"


void pthreadErrorChecker(int retVal)
{
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_create() is %d\n", retVal);
		exit(ERROR);
	}
}


void pthreadCreate(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg)
{	
	int retVal = pthread_create(thread, attr, start_routine, arg);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_create() is %d\n", retVal);
		exit(ERROR);
	}
}

void pthreadMutexInit(pthread_mutex_t *restrict mutex)
{	
	int retVal = pthread_mutex_init(mutex, NULL);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_mutex_init is %d\n", retVal);
		exit(ERROR);
	}
}


void pthreadCondInit(pthread_cond_t *restrict cond)
{	
	int retVal = pthread_cond_init(cond, NULL);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_cond_init is %d\n", retVal);
		exit(ERROR);
	}
}


void pthreadMutexLock(pthread_mutex_t *mutex)
{	
	int retVal = pthread_mutex_lock(mutex);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_mutex_lock is %d\n", retVal);
		exit(ERROR);
	}
}

void pthreadMutexUnlock(pthread_mutex_t *mutex)
{	
	int retVal = pthread_mutex_unlock(mutex);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_mutex_unlock is %d\n", retVal);
		exit(ERROR);
	}
}



void pthreadCondWait(pthread_cond_t *restrict cond,
              pthread_mutex_t *restrict mutex)
{	
	int retVal = pthread_cond_wait(cond, mutex);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_cond_wait is %d\n", retVal);
		exit(ERROR);
	}
}





void pthreadCondSignal(pthread_cond_t *cond)
{	
	int retVal = pthread_cond_signal(cond);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_cond_signal is %d\n", retVal);
		exit(ERROR);
	}
}




void pthreadJoin(pthread_t thread)
{	
	int retVal = pthread_join(thread, NULL);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_join is %d\n", retVal);
		exit(ERROR);
	}
}





void pthreadCondDestroy(pthread_cond_t *cond)
{	
	int retVal = pthread_cond_destroy(cond);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_cond_destroy is %d\n", retVal);
		exit(ERROR);
	}
}


void pthreadMutexDestroy(pthread_mutex_t *mutex)
{	
	int retVal = pthread_mutex_destroy(mutex);
	if(retVal)//any value other 0 is an error
	{	
		printf("ERROR - return code from pthread_mutex_destroy is %d\n", retVal);
		exit(ERROR);
	}
}