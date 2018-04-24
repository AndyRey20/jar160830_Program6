# Julian Reyes jar160830@utdallas.edu CS 3377.501
# 
# Makefile



CXX = g++
CXXFLAGS = -Wall -g

CPPFLAGS = -I /scratch/perkins/include
LDFLAGS = -L /scratch/perkins/lib
LDLIBS = -l cdk -l curses


PROJECTNAME = CS3377.Program6.jar160830

EXECFILE = cdkexample

OBJS = cdkexample.o




all: $(EXECFILE)

clean:
	rm -f $(OBJS) $(EXECFILE) *.P *~ \#*

$(EXECFILE): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

backup:
	@make clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename "`pwd`"))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup files to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME) 2> /dev/null
	@echo
	@echo Done!
