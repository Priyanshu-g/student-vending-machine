TESTCASE := NOTEST
CXX = u++					# uC++ compiler
#CXX = /u/cs343/cfa-cc/bin/cfa			# CFA compiler
CXXFLAGS = -g -Wall -Wextra -multi -MMD -D"${TESTCASE}" # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = main.o config.o printer.o bank.o parent.o watcard.o watcardoffice.o groupoff.o nameserver.o vendingmachine.o bottlingplant.o truck.o student.o
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXEC = soda

# ${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d ${OBJECTS} ${EXEC}