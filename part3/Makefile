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

BINS = pr3-01 pr3-02 pr3-03 pr3-04 pr3-05 pr3-06 pr3-07 pr3-08 pr3-09 pr3-10 pr3-11 pr3-12 pr3-13 pr3-14

all: $(BINS)

pr3-01: pr3-01.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-02: pr3-02.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-03: pr3-03.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-04: pr3-04.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-05: pr3-05.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-06: pr3-06.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-07: pr3-07.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-08: pr3-08.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-09: pr3-09.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-10: pr3-10.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-11: pr3-11.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-12: pr3-12.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-13: pr3-13.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr3-14: pr3-14.c
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	$(RM) $(RM_FLAGS) $(BINS) $(addsuffix .exe,$(BINS))

