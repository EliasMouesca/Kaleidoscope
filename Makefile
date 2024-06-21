CFLAGS=-Wall
LDFLAGS=-lm -lSDL2 -lSDL2main -lSDL2_image
EXECUTABLE=kaleidoscope
CC=g++

OBJDIR=./obj
SRCDIR=./src

SRCFILES:=$(wildcard $(SRCDIR)/*.cpp)
OBJFILES:=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCFILES))

all: $(OBJDIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJFILES)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/kaleidoscope.o: $(SRCDIR)/kaleidoscope.cpp $(SRCDIR)/kaleidoscope.h
	$(CC) -ggdb -c -o $@ $< $(CFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) -ggdb -c -o $@ $< $(CFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

perf: $(EXECUTABLE)
	perf record -o perf/perf.data $(EXECUTABLE)

clean:
	rm -f $(OBJDIR)/*

.PHONY:
	clean
	perf

.SILENT:
	clean

