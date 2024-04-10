TARGET=algo
TARGET_SO=algo.so
LIBS=-L. -l:$(TARGET_SO)
CC=gcc
CFLAGS=-g -I. -fsigned-char -fPIC -std=c17 -D__LINUX__
LFLAGS=-Wall
LFLAGS_SO=-Wall -shared
OBJECTS=$(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS_SO=$(filter-out main.o,$(OBJECTS))	
HEADERS=$(wildcard *.h)
OBJS=main.o

default: $(TARGET) $(TARGET_SO)

# Link executable
$(TARGET) : $(TARGET_SO) $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(LIBS) -o $@

# Link shared library
$(TARGET_SO) : $(OBJECTS_SO)
	$(CC) $(OBJECTS_SO) $(LFLAGS_SO) -o $@

# Compile
%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f $(TARGET_SO)
