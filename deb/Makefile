# Define the compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-Wall -Wextra -pedantic

# Define any directories containing header files
INCLUDES=

# Define library paths in addition to /usr/lib
LFLAGS=

# Define any libraries to link into executable
LIBS=

# Define the C source files
SRCS=main.c

# Define the C object files
OBJS=$(SRCS:.c=.o)

# Define the executable file
MAIN=vcan

.PHONY: depend clean

all:    $(MAIN)
	@echo  Program has been compiled

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# This is a suffix replacement rule for building .o's from .c's
# It uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule(a .o file)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

