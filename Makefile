#
# Written by George D. Sotirov, astronom@dir.bg
#

SUBDIRS = part2 part3 part4 part5 part6 part7 part8
# Uncoment the folowing line to enable debug builds
#export DEBUG = 1

all:
	@for DIR in $(SUBDIRS); do  \
	echo "Making $@ in $$DIR" ; \
	cd $$DIR && $(MAKE) $@    ; \
	cd ..                     ; \
	done

clean:
	@for DIR in $(SUBDIRS); do  \
	echo "Making $@ in $$DIR" ; \
	cd $$DIR && $(MAKE) $@    ; \
	cd ..                     ; \
	done

p2:
	@$(MAKE) -C part2

p3:
	@$(MAKE) -C part3

p4:
	@$(MAKE) -C part4

p5:
	@$(MAKE) -C part5

p6:
	@$(MAKE) -C part6

p7:
	@$(MAKE) -C part7

p8:
	@$(MAKE) -C part8

part2-cl:
	@cd `echo $@ | sed s/-cl//` && $(MAKE) clean

part3-cl:
	@cd `echo $@ | sed s/-cl//` && $(MAKE) clean

part4-cl:
	@cd `echo $@ | sed s/-cl//` && $(MAKE) clean

part5-cl:
	@cd `echo $@ | sed s/-cl//` && $(MAKE) clean

part6-cl:
	@cd `echo $@ | sed s/-cl//` && $(MAKE) clean

part7-cl:
	@cd `echo $@ | sed s/-cl//` && $(MAKE) clean

part8-cl:
	@cd `echo $@ | sed s/-cl//` && $(MAKE) clean

help:
	@echo "Available targets:"
	@echo "all                  - build all examples"
	@echo "clean                - clean all binaries"
	@echo "p1 .. p8             - build examples form part n"
	@echo "part1-cl .. part8-cl - clean binaries from part n"

