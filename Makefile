CFLAGS=-Wall
LDFLAGS=-lSDL2 -lSDL2main -lSDL2_image
EXECUTABLE=kaleidoscope

OBJDIR=./obj
SRCDIR=./src

SRCFILES:=$(wildcard $(SRCDIR)/*.c)
OBJFILES:=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))

$(EXECUTABLE): $(OBJFILES)
	gcc-o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	gcc -ggdb -c -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJDIR)/*

.PHONY:
	clean

.SILENT:
	clean

