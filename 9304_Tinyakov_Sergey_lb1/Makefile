CC = gcc
FLAGS = -DTIME_OUTPUT
SRC_DIR = ./src
BIN_GEN = generate_matrix
BIN_GEN_DEP = $(SRC_DIR)/generate_matrix.c
BIN_READ = read_elem
BIN_READ_DEP = $(SRC_DIR)/read_element_from_matrix.c
BIN_PROCESSES = process_sum
BIN_PROCESSES_DEPS = $(SRC_DIR)/process_processing.c \
				   $(SRC_DIR)/read_write.c \
				   $(SRC_DIR)/matrix_operations.c
BIN_PROCESSES_HEADERS = $(SRC_DIR)/read_write.h \
				        $(SRC_DIR)/matrix_operations.h
BIN_THREADS = thread_sum
BIN_THREADS_DEPS = $(SRC_DIR)/thread_processing.c \
				   $(SRC_DIR)/read_write.c \
				   $(SRC_DIR)/matrix_operations.c
BIN_THREADS_HEADERS = $(SRC_DIR)/read_write.h \
				      $(SRC_DIR)/matrix_operations.h \
					  $(SRC_DIR)/data.h

ALL_BINS = $(BIN_GEN) $(BIN_READ) $(BIN_PROCESSES) $(BIN_THREADS)

.PHONY: all clean remove_matrices

all: $(ALL_BINS)

$(BIN_GEN): $(BIN_GEN_DEP)
	$(CC) $(FLAGS) $(BIN_GEN_DEP) -o $@

$(BIN_READ): $(BIN_READ_DEP)
	$(CC) $(FLAGS) $(BIN_READ_DEP) -o $@

$(BIN_PROCESSES): $(BIN_PROCESSES_DEPS) $(BIN_PROCESSES_HEADERS)
	$(CC) $(FLAGS) $(BIN_PROCESSES_DEPS) -o $@

$(BIN_THREADS): $(BIN_THREADS_DEPS) $(BIN_THREADS_HEADERS)
	$(CC) $(FLAGS) $(BIN_THREADS_DEPS) -lpthread -o $@

clean:
	rm -f *.o *.bin *.mat *.log $(ALL_BINS)

remove_matrices:
	rm -f *.o *.bin *.mat
