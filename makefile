# this is a makefile of the c project myvi
# the standard c compiler on GNU linux is gcc
# written by yuhai on May 10th, 2013

                    TARGET = .

                        GCC = gcc
                     CFLAGS = -g -Wall
                     CFLAGC = -c

                        SRC = myvi.c \
                              edit.c \
                              commend.c \
                              save.c
                       OBJS = myvi.o \
                              edit.o \
                              commend.o \
                              save.o
                       EXEC = myvi

                    INCLUDE =  
                       LIBS = -lncurses

all: $(EXEC)

$(EXEC): $(OBJS)
	$(GCC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

$(OBJS): $(SRC)
	$(GCC) $(CFLAGC) $(SRC) $(INCLUDE)

install:
	cp $(EXEC) /usr/local/bin

uninstall:
	rm -f /usr/local/bin/$(EXEC)

clean:
	rm -f $(EXEC)
	rm -f $(OBJS)
