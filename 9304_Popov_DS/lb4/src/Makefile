CC 		= g++
CFLAGS  = -c -Wall -g -std=c++17
LDFLAGS = -pthread
SOURCES = main.cpp ThreadLab.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET  = lab4


.PHONY: all clean


all: $(TARGET) $(SOURCES) 


$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@
	@chmod +x $@
	@echo "\033[92m$(TARGET) assembled successfully!\033[0m"

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@



clean:
	@$(RM) $(TARGET) $(OBJECTS) *.txt
	@echo "\033[1;31m$(TARGET) $(OBJECTS) deleted!\033[0m"