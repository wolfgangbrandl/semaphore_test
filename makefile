CC              =       /usr/bin/gcc
CC2             =       /usr/bin/gcc
DATASOURCE      =       TAISDB
QUALIFIER       =       TAEDB
USER		=	db2it01
PW		=	harak1Ri0
DB2INSTANCEPATH =       /home/$(DB2INSTANCE)
CFLAGS          =       -g -w  -fvisibility=hidden -Wunused -Wall
LIBDIR          =       -L$(DB2LIB) -ldb2 -L.
INCLUDE         =       -I$(DB2_HOME)/include -I./
SHELL           =       /bin/bash
REPLVER         :=      $(shell date +%y%m%d%H%M%S)


all: semVdemo semVinit semVrm semPdemo semPdemoParent semPdemoChild semBinary semabinit sema semb
semBinary.o:  semBinary.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
semBinary: semBinary.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $? 
semPdemo.o:  semPdemo.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
semPdemo: semPdemo.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $? -lpthread

semVdemo.o:  semVdemo.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
semVdemo: semVdemo.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $? 

semVinit.o:  semVinit.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
semVinit: semVinit.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $?

semVrm.o:  semVrm.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
semVrm: semVrm.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $?

semPdemoParent.o:  semPdemoParent.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
semPdemoParent: semPdemoParent.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $? -lpthread
semPdemoChild.o:  semPdemoChild.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
semPdemoChild: semPdemoChild.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $? -lpthread

semabinit.o:  semabinit.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
semabinit: semabinit.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $?
sema.o:  sema.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
sema: sema.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $?
semb.o:  semb.c
	$(CC) -g -c $? $(INCLUDE) -fPIC
semb: semb.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBDIR) -o $@ $?

testB:
	echo Posix Thread
	date
	hostname
	lparstat -i|grep Type
	./semBinary
testP:
	echo Posix Thread
	date
	hostname
	lparstat -i|grep Type
	./semPdemoParent
testV:
	echo System V Thread
	date
	hostname
	lparstat -i|grep Type
	./semVinit;./semVdemo;./semVrm
packsource:
	tar cvf ./semaphore.tar ./semVdemo.c ./semVinit.c ./semVrm.c ./semPdemo.c ./semPdemoParent.c ./semPdemoChild.c ./makefile
packexe:
	tar cvf ./semaphore_exe.tar ./semVdemo ./semVinit ./semVrm ./semPdemo ./semPdemoParent ./semPdemoChild ./makefile
clean:
	rm -f *.o semVdemo semVinit semVrm semPdemo semPdemoParent semPdemoChild semVdemo semVinit semVrm semPdemo semPdemoParent semPdemoChild semBinary semabinit sema semb *.tar

checkin:
	git remote set-url origin https://brandlw@git.brz.gv.at/bitbucket/scm/izsdbdb2/semaphore_test.git
	git add --all
	git commit -m "`date`"
	git push -u origin master
