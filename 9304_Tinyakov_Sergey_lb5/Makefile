CC = gcc
CFLAGS = -lpthread -g -lOpenCL
TMP = tmp
SRC_DIR = src
COLOR = \033[0;32m
NORMAL = \033[0m
PMM = pmm
PMM_THREADS_DEPS = $(TMP)/main.o \
			  	   $(TMP)/matrix_operations.o \
				   $(TMP)/parallel_matrix_multiplication.o \
				   $(TMP)/gpu_matrix_multiplication.o
PMM_CHECK = check
PMM_CHECK_THREADS_DEPS = $(TMP)/check.o \
						 $(TMP)/matrix_operations.o \
						 $(TMP)/parallel_matrix_multiplication.o \
						 $(TMP)/gpu_matrix_multiplication.o

ALL_BINS = $(PMM) $(PMM_CHECK)

.PHONY: all clean remove_matrices

all: $(ALL_BINS)


$(TMP)/%.d: $(SRC_DIR)/%.c | $(TMP)
	@echo "$(COLOR)Creating $< dependency...$(NORMAL)"
	$(CC) -MM -MT $@ $(CFLAGS) $< > $@

$(TMP)/%.o: $(SRC_DIR)/%.c $(TMP)/%.d | $(TMP)
	@echo "$(COLOR)Compiling $@...$(NORMAL)"
	$(CC) -c $(CFLAGS) $< -o $@

$(PMM): $(PMM_THREADS_DEPS)
	@echo "$(COLOR)Linking programm $@...$(NORMAL)"
	$(CC) $^ -o $@ $(CFLAGS)

$(PMM_CHECK): $(PMM_CHECK_THREADS_DEPS)
	@echo "$(COLOR)Linking programm $@...$(NORMAL)"
	$(CC) $^ -o $@ $(CFLAGS)

$(TMP):
	@mkdir -p $(TMP)

clean:
	@echo "$(COLOR)Cleanning...$(NORMAL)"    
	rm -rf $(TMP) $(ALL_BINS)

include $(PMM_THREADS_DEPS:.o=.d) 
include $(PMM_CHECK_THREADS_DEPS:.o=.d) 
