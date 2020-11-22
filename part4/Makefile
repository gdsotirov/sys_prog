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

BINS = pr4-01

all: $(BINS)

pr4-01: pr4-01.c
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	$(RM) $(RM_FLAGS) $(BINS) $(addsuffix .exe,$(BINS))

