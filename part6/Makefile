#
# Written by George D. Sotirov, astronom@dir.bg
#

CC = gcc
ifeq ($(DEBUG), 1)
    CC_FLAGS = -Wall -ggdb
else
    CC_FLAGS = -Wall
endif
RM = rm
RM_FLAGS = -f

BINS = pr6-01 pr6-02 pr6-03 pr6-04 pr6-05 pr6-06 pr6-07 pr6-08 pr6-09 pr6-10 pr6-11 pr6-12

all: $(BINS)

pr6-01: pr6-01.c msgbuf.h
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-02: pr6-02.c msgbuf.h
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-03: pr6-03.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-04: pr6-04.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-05: pr6-05.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-06: pr6-06.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-07: pr6-07.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-08: pr6-08.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-09: pr6-09.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-10: pr6-10.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-11: pr6-11.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr6-12: pr6-12.c
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	$(RM) $(RM_FLAGS) $(BINS)

