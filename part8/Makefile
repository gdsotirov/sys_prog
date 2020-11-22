#
# Written by Georgi D. Sotirov, gdsotirov@gmail.com
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
BINS = pr8-01

all: $(BINS)
	
pr8-01: thr_lib.o pr8-01.o
	$(CC) $(CCFLAGS) $(OBJS) -o $@

thr_lib.o: thr_lib.c thr_def.h
	$(CC) $(CCFLAGS) -o $@ -c $<

pr8-01.o: pr8-01.c
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	$(RM) $(RMFLAGS) $(OBJS) $(BINS) $(addsuffix .exe,$(BINS))

