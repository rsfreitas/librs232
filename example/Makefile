
CC = gcc
TARGET = rs232

INCLUDEDIR = -I../include
CFLAGS = -Wall -O0 -ggdb -fgnu89-inline -D_GNU_SOURCE $(INCLUDEDIR)

LIBDIR = -L/usr/local/lib
LIBS = -lrs232

OBJECTS =	\
	example.o

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBDIR) $(LIBS)

clean:
	rm -rf $(OBJECTS) $(TARGET)

