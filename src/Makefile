INCDIR =../include
CC=gcc
CFLAGS=-I$(INCDIR) -Wall

SRCDIR = ./
ODIR=obj
LDIR =../lib
TARGETDIR = ../bin

TARGET = bcsh #Stands for Brennan Couturier's Shell

_DEPS = $(shell find $(INCDIR) -type f -name '*.h')
DEPS = $(patsubst %, $(INCDIR)/%, $(_DEPS))

_SRCS = $(shell find -L $(SRCDIR) -type f -name '*.c' -exec basename {} ';') #finds all the .c files and gives back filename.c to fill _SRCS
_OBJ = $(_SRCS:.c=.o)
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGETDIR)/$@ $^ $(CFLAGS)

.PHONY: clean run debug valgrind

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(TARGETDIR)/*

run: $(TARGET)
	$(TARGETDIR)/$^

debug: $(OBJ)
	$(CC) -g -o $(TARGETDIR)/$@ $^ $(CFLAGS)
	@gdb ../bin/debug

valgrind: $(TARGET)
	@valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind-out.txt \
			../bin/bcsh