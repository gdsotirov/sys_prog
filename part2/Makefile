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

BINS = pr2-01 pr2-02 pr2-03 pr2-04 pr2-05 pr2-06 pr2-07

all: $(BINS)

pr2-01: pr2-01.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr2-02: pr2-02.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr2-03: pr2-03.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr2-04: pr2-04.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr2-05: pr2-05.c
	$(CC) $(CC_FLAGS) -D_XOPEN_SOURCE=500 -o $@ $<

pr2-06: pr2-06.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr2-07: pr2-07.c
	$(CC) $(CC_FLAGS) -D_XOPEN_SOURCE=500 -o $@ $<

clean:
	$(RM) $(RM_FLAGS) $(BINS) $(addsuffix .exe,$(BINS))

