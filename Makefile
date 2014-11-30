OPT:=-O2 -multi

CXX = u++ # compiler
CXXFLAGS = -std=c++0x -g -Wall -Wno-unused-label -MMD ${OPT}
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}} # makefile name

OBJECTS1 = a6bank.o a6bottlingplant.o a6config.o a6main.o a6nameserver.o a6parent.o a6printer.o \
	a6student.o a6truck.o a6vendingmachine.o a6watcard.o a6watcardoffice.o \
	utilities.o
EXEC1 = soda

OBJECTS = ${OBJECTS1} # all object files
DEPENDS = ${OBJECTS:.o=.d} # substitute ".o" with ".d"
EXECS = ${EXEC1} # all executables

.PHONY : all clean

all : ${EXECS} # build all executables

${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}

clean: # remove files that can be regenerated
	rm -f *.d *.o ${EXECS}

style:
	./astyle/build/gcc/bin/astyle --style=java --lineend=linux --indent=spaces --suffix=none --add-one-line-brackets --add-brackets --convert-tabs --pad-paren-in --align-reference=type --align-pointer=type --max-code-length=100 --close-templates --mode=cs *.cc *.h
