CC 		= g++
CFLAGS  = -c -Wall -g -std=c++17
LDFLAGS = -pthread -lOpenCL
SOURCES = main.cpp matrix.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET  = lab5


.PHONY: all clean


all: $(TARGET) $(SOURCES) 


$(TARGET): $(OBJECTS)
	$(CC)  $^ -o $@ $(LDFLAGS)
	@chmod +x $@
	@echo "\033[92m$(TARGET) assembled successfully!\033[0m"

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@



clean:
	@$(RM) $(TARGET) $(OBJECTS) *.txt
	@echo "\033[1;31m$(TARGET) $(OBJECTS) deleted!\033[0m"