CC = g++
CFLAGS = -std=c++0x
LDFLAGS = 
OBJFILES = main.o
TARGET = compilador
HEADERS = lexico.h parser.h semantic.h
C_FILES = $(wildcard ls ./casos/*.c)
S_PATH = $(wildcard ./saidas/)


all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
$(OBJFILES): main.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c main.cpp	
clean:
	rm -f $(OBJFILES) $(TARGET) *~	
	$(MAKE) all

run:
	$(MAKE) all
	@n=1;
	#@var
	#touch ${S_PATH}$$n.out;	
	#touch ${S_PATH}$$((n+=1)).out
	for f in ${C_FILES}; do ./$(TARGET) $$f ${S_PATH}$$(echo $$f | cut -d'/' -f 3);  done
	
show_vars:    
	@echo ${C_FILES}	
