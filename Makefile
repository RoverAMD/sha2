CC ?= cc
CFLAGS = -Wall -Wextra -std=c99 -I.
CDEFINES = -DUNROLL_LOOPS
AR ?= ar
TARGET = libsha2.a

all: $(TARGET)

lib: $(TARGET)

$(TARGET): sha2.o
	$(AR) crs $(TARGET) sha2.c

sha2.o: sha2.c sha2.h
	$(CC) $(CFLAGS) -o $@ $(CDEFINES) -c $<

distclean: clean

clean:
	rm -rf *.o $(TARGET)


