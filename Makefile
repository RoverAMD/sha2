CC ?= cc
CFLAGS = -Wall -Wextra -std=c99 -I.
CDEFINES = -DUNROLL_LOOPS -DALLOW_EASYSHA_WARNINGS
AR ?= ar
TARGET = libsha2.a
CLITARGET = easysha

all: $(TARGET) $(CLITARGET)

lib: $(TARGET)

cli: $(CLITARGET)

$(CLITARGET): $(TARGET)
	$(CC) -o $(CLITARGET) $(CFLAGS) cli.c $(TARGET)

$(TARGET): sha2.o
	$(AR) crs $(TARGET) sha2.o

sha2.o: sha2.c sha2.h
	$(CC) $(CFLAGS) -o $@ $(CDEFINES) -c $<

distclean: clean

clean:
	rm -rf *.o $(TARGET) $(CLITARGET)
