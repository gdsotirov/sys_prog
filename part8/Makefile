#
# Written by George D. Sotirov, astronom@dir.bg
#

CC = gcc
ifeq ($(DEBUG), 1)
    CCFLAGS = -Wall -ggdb
else
	CCFLAGS = -Wall
endif
RM = rm
RMFLAGS = -f

OBJS = thr_lib.o pr8-01.o

all: pr8-01
	
pr8-01: thr_lib.o pr8-01.o
	$(CC) $(CCFLAGS) $(OBJS) -o $@

thr_lib.o: thr_lib.c thr_def.h
	$(CC) $(CCFLAGS) -o $@ -c $<

pr8-01.o: pr8-01.c
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	$(RM) $(RMFLAGS) $(OBJS) pr8-01

