CC = gcc
CFLAGS = -lpthread -march=native -latomic -g
TMP = tmp
SRC_DIR = src
COLOR = \033[0;32m
NORMAL = \033[0m
BIN_THREADS = thread_sum
BIN_THREADS_DEPS = $(TMP)/main.o \
			  	   $(TMP)/writer.o \
			  	   $(TMP)/summator.o \
				   $(TMP)/generation.o \
				   $(TMP)/list.o \
				   $(TMP)/non_blocking_list.o \
				   $(TMP)/matrix_operations.o
BIN_TEST_TIME = test_time
BIN_TEST_TIME_DEPS = $(TMP)/time_test.o \
					 $(TMP)/list.o \
					 $(TMP)/non_blocking_list.o \
					 $(TMP)/non_blocking_cycled_list.o

ALL_BINS = $(BIN_TEST_TIME) $(BIN_THREADS)

.PHONY: all clean remove_matrices

all: $(ALL_BINS)


$(TMP)/%.d: $(SRC_DIR)/%.c | $(TMP)
	@echo "$(COLOR)Creating $< dependency...$(NORMAL)"
	rm -rf $@
	$(CC) -MM -MT $@ $(CFLAGS) $< > $@

$(TMP)/%.o: $(SRC_DIR)/%.c $(TMP)/%.d | $(TMP)
	@echo "$(COLOR)Compiling $@...$(NORMAL)"
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN_THREADS): $(BIN_THREADS_DEPS)
	@echo "$(COLOR)Linking programm $@...$(NORMAL)"
	$(CC) $^ -o $@ $(CFLAGS)

$(BIN_TEST_TIME): $(BIN_TEST_TIME_DEPS)
	@echo "$(COLOR)Linking programm $@...$(NORMAL)"
	$(CC) $^ -o $@ $(CFLAGS)

$(TMP):
	@mkdir -p $(TMP)

clean:
	@echo "$(COLOR)Cleanning...$(NORMAL)"    
	rm -rf $(TMP) $(ALL_BINS)

# include $(OBJ:.o=.d) 