/*
 Purpose: wrappers for calls to pthread library, to exit when error happens
*/


#ifndef THREADWRAPPERS_H
#define THREADWRAPPERS_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define ERROR -1
void pthreadErrorChecker(int retVal);

void pthreadCreate(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
void pthreadMutexInit(pthread_mutex_t *restrict mutex);
void pthreadCondInit(pthread_cond_t *restrict cond);
void pthreadMutexLock(pthread_mutex_t *mutex);
void pthreadMutexUnlock(pthread_mutex_t *mutex);
void pthreadCondWait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
void pthreadCondSignal(pthread_cond_t *cond);
void pthreadJoin(pthread_t thread);
void pthreadCondDestroy(pthread_cond_t *cond);
void pthreadMutexDestroy(pthread_mutex_t *mutex);
void pthreadErrorChecker(int retVal);
#endif
