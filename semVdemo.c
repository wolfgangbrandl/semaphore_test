#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>

double                         dTimeLock;
double                         dTimeLockSum;
double                         dTimeUnlock;
double                         dTimeUnlockSum;

#define LOOPCOUNT   1000000

/*************************************************************************/
/*************************************************************************/
void tstart (struct timeval * _tstart,struct timezone * tz)
{
  gettimeofday (_tstart, tz);
}
/*************************************************************************/
/*************************************************************************/
void tend (struct timeval * _tend,struct timezone * tz)
{
  gettimeofday (_tend, tz);
}
/*************************************************************************/
/*************************************************************************/
double tval (struct timeval * _tstart,struct timeval * _tend,struct timezone * tz)
{
  double t1, t2;
  t1 = (double) _tstart->tv_sec + (double) _tstart->tv_usec/(1000*1000);
  t2 = (double) _tend->tv_sec + (double) _tend->tv_usec/(1000*1000);
  return t2-t1;
}

/*************************************************************************/
/*************************************************************************/

int main(void)
{
  struct timeval _tstart;
  struct timeval _tend;
  struct timezone tz;

  key_t key;
  int semid;
  dTimeLock = 0.0;
  dTimeLockSum = 0.0;
  dTimeUnlock = 0.0;
  dTimeUnlockSum = 0.0;
  int i;

  for ( i=0; i < LOOPCOUNT;i++)
  {
    tstart(&_tstart,&tz);
  
    struct sembuf sb = {0, 1, 0}; /* set to allocate resource */
    if ((key = ftok("semVdemo", 'J')) == -1) {
      perror("ftok");
      exit(1);
    }
    /* grab the semaphore set created by seminit.c: */
    if ((semid = semget(key, 1, 0)) == -1) {
      perror("semget");
      exit(1);
    } 
  
    if (semop(semid, &sb, 1) == -1) {
      perror("semop");
      exit(1);
    }
    tend(&_tend,&tz);
    dTimeLock = tval(&_tstart,&_tend,&tz);
    dTimeLockSum = dTimeLockSum + dTimeLock;
  
  
    tstart(&_tstart,&tz);
  
    sb.sem_op = -1; /* free resource */
    if (semop(semid, &sb, 1) == -1) {
      perror("semop");
      exit(1);
    }
    tend(&_tend,&tz);
    dTimeUnlock = tval(&_tstart,&_tend,&tz);
    dTimeUnlockSum = dTimeUnlockSum + dTimeUnlock;
  }
  printf (" System V Semaphore LockTime     %f AVG:%f\n",dTimeLockSum,dTimeLockSum/LOOPCOUNT);
  printf (" System V Semaphore UnLockTime   %f AVG:%f\n",dTimeUnlockSum,dTimeUnlockSum/LOOPCOUNT);
  return 0;
}
