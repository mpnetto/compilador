CC = g++
CFLAGS = -std=c++0x
LDFLAGS =
OBJFILES = main.o
TARGET = compilador
HEADERS = lexico.h parser.h semantic.h


all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
$(OBJFILES): main.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c main.cpp
clean:
	rm -f $(OBJFILES) $(TARGET) *~


