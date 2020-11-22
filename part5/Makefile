#
# Written by Georgi D. Sotirov, gdsotirov@gmail.com
#

CC = gcc
ifeq ($(DEBUG), 1)
    CC_FLAGS = -Wall -ggdb
else
    CC_FLAGS = -Wall
endif
RM = rm
RM_FLAGS = -f

BINS = pr5-01 pr5-02 pr5-03 pr5-04 pr5-05 pr5-06 pr5-07 pr5-08 pr5-09

all: $(BINS)

pr5-01: pr5-01.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr5-02: pr5-02.c
	$(CC) $(CC_FLAGS) -D_XOPEN_SOURCE=500 -o $@ $<

pr5-03: pr5-03.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr5-04: pr5-04.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr5-05: pr5-05.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr5-06: pr5-06.c
	$(CC) $(CC_FLAGS) -D_XOPEN_SOURCE=500 -o $@ $<

pr5-07: pr5-07.c
	$(CC) $(CC_FLAGS) -D_XOPEN_SOURCE=500 -o $@ $<

pr5-08: pr5-08.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr5-09: pr5-09.c
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	$(RM) $(RM_FLAGS) $(BINS) $(addsuffix .exe,$(BINS))

