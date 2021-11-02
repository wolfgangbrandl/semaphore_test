#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
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

  msemaphore Sem;
  msem_init (&Sem, MSEM_UNLOCKED);


  dTimeLock = 0.0;
  dTimeLockSum = 0.0;
  dTimeUnlock = 0.0;
  dTimeUnlockSum = 0.0;
  int i;

  for ( i=0; i < LOOPCOUNT;i++)
  {
    tstart(&_tstart,&tz);
    msem_lock (&Sem,MSEM_LOCKED);
    tend(&_tend,&tz);
    dTimeLock = tval(&_tstart,&_tend,&tz);
    dTimeLockSum = dTimeLockSum + dTimeLock;
  
  
    tstart(&_tstart,&tz);
    msem_unlock (&Sem,MSEM_UNLOCKED);
  
    tend(&_tend,&tz);
    dTimeUnlock = tval(&_tstart,&_tend,&tz);
    dTimeUnlockSum = dTimeUnlockSum + dTimeUnlock;
  }
  printf (" AIX Binary Lock LockTime   %f AVG:%f\n",dTimeLockSum,dTimeLockSum/LOOPCOUNT);
  printf (" AIX Binary Lock UnLockTime %f AVG:%f\n",dTimeUnlockSum,dTimeUnlockSum/LOOPCOUNT);
  return 0;
}
