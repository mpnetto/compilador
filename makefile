CC = g++
CFLAGS = -std=c++0x
LDFLAGS =
OBJFILES = main.o
TARGET = compilador
HEADERS = lexico.h parser.h
C_FILES = $(wildcard ./casos/*.c)



all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
$(OBJFILES): main.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c main.cpp
clean:
	rm -f $(OBJFILES) $(TARGET) *~

run:
	$(MAKE) all
	for f in ${C_FILES}; do ./$(TARGET) $$f; done

show_vars:
	@echo ${C_FILES}
