#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>


double                         dTimeLock;
double                         dTimeLockSum;
double                         dTimeUnlock;
double                         dTimeUnlockSum;

#define LOOPCOUNT   1000000
#define SEM_NAME "/tmp/semaphore_example"
#define ITERS 10

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
  int i;

  sem_t *semaphore = sem_open(SEM_NAME, O_RDWR);
  if (semaphore == SEM_FAILED) {
     perror("sem_open(3) failed");
        exit(EXIT_FAILURE);
  }

  for (i = 0; i < LOOPCOUNT; i++) {
    tstart(&_tstart,&tz);
    if (sem_wait(semaphore) < 0) {
      perror("sem_wait(3) failed on child");
      continue;
    }
    tend(&_tend,&tz);
    dTimeLock = tval(&_tstart,&_tend,&tz);
    dTimeLockSum = dTimeLockSum + dTimeLock;

/*    printf("PID %ld acquired semaphore\n", (long) getpid());*/

    tstart(&_tstart,&tz);
    if (sem_post(semaphore) < 0) {
        perror("sem_post(3) error on child");
    }
    tend(&_tend,&tz);
    dTimeUnlock = tval(&_tstart,&_tend,&tz);
    dTimeUnlockSum = dTimeUnlockSum + dTimeUnlock;
  }

  if (sem_close(semaphore) < 0)
    perror("sem_close(3) failed");

  printf (" Posix Child Semaphore LockTime     %f AVG:%f\n",dTimeLockSum,dTimeLockSum/LOOPCOUNT);
  printf (" Posix Child Semaphore UnLockTime   %f AVG:%f\n",dTimeUnlockSum,dTimeUnlockSum/LOOPCOUNT);

  return 0;
}
