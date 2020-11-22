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

BINS = pr7-01 pr7-02

all: $(BINS)

pr7-01: pr7-01.c
	$(CC) $(CC_FLAGS) -o $@ $<

pr7-02: pr7-02.c
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	$(RM) $(RM_FLAGS) $(BINS) $(addsuffix .exe,$(BINS))

