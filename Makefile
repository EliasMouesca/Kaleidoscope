CFLAGS=-Wall
LDFLAGS=-lm -lSDL2 -lSDL2main -lSDL2_image
EXECUTABLE=kaleidoscope

OBJDIR=./obj
SRCDIR=./src

SRCFILES:=$(wildcard $(SRCDIR)/*.c)
OBJFILES:=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))

$(EXECUTABLE): $(OBJFILES)
	gcc -o $@ $^ $(LDFLAGS)

test: test.c $(SRCDIR)/kaleidoscope.c
	gcc $^ -o test $(LDFLAGS) -lpthread -ggdb

$(OBJDIR)/kaleidoscope.o: $(SRCDIR)/kaleidoscope.c 
	gcc -ggdb -c -o $@ $^ $(CFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/Constants.h 
	gcc -ggdb -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJDIR)/*

.PHONY:
	clean

.SILENT:
	clean

