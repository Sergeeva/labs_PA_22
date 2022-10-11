CC = g++

CFLAGS  = -g -Wall

TARGET = main
QUEUE = queue
MATRIX = matrix

all: $(TARGET)

$(TARGET): $(TARGET).o $(QUEUE).o $(MATRIX).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(QUEUE).o  $(MATRIX).o

$(TARGET).o: $(TARGET).cpp $(QUEUE).hpp $(MATRIX).hpp
	$(CC) -c -std=c++14 -pthread $(TARGET).cpp

$(QUEUE).o: $(QUEUE).cpp $(QUEUE).hpp $(MATRIX).hpp
	$(CC) -c -std=c++14 -pthread $(QUEUE).cpp

$(MATRIX).o: $(MATRIX).cpp $(MATRIX).hpp
	$(CC) -c -std=c++14 -pthread $(MATRIX).cpp

clean:
	rm -f $(BIN) *.o
