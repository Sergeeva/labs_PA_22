CC = g++

CFLAGS  = -g -Wall

TARGET = main
BUFFER = buffer
MATRIX = matrix

all: $(TARGET)

$(TARGET): $(TARGET).o $(BUFFER).o $(MATRIX).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(BUFFER).o $(MATRIX).o

$(TARGET).o: $(TARGET).cpp $(BUFFER).hpp $(MATRIX).hpp
	$(CC) -c -std=c++14 -pthread $(TARGET).cpp

$(BUFFER).o: $(BUFFER).cpp $(BUFFER).hpp $(MATRIX).hpp
	$(CC) -c -std=c++14 -pthread $(BUFFER).cpp

$(MATRIX).o: $(MATRIX).cpp $(MATRIX).hpp
	$(CC) -c -std=c++14 -pthread $(MATRIX).cpp

clean:
	rm -f $(BIN) *.o
