SRC_DIR = ./src
INC_DIR = ./include
BIN_DIR = ./bin

TMP_DIR = ./tmp
REL_DIR = ./lib

SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
OUTPUT = $(BIN_DIR)/bin

CFLAGS = -Wall -Wextra -O0 -g3

VALGRIND_OUT = ./valgrind/analysis.out
CALLGRIND_FLAGS = --tool=callgrind --dump-line=yes --dump-instr=yes --collect-jumps=yes --collect-systime=yes --collect-bus=yes --cache-sim=yes --branch-sim=yes --simulate-wb=yes --simulate-hwpref=yes --cacheuse=yes --time-stamp=yes

CC = gcc
AR = ar
RANLIB = ranlib

#CFLAGS += -DSTATIC_ALLOCATOR_DEBUG_MODE
#CFLAGS += -DMEMORY_DIRECTORY_DEBUG_MODE

all:
	$(CC) $(SRC) -o $(OUTPUT) -I$(INC_DIR) $(CFLAGS)

mcall:
	$(CC) $(SRC) -o $(OUTPUT) -I$(INC_DIR) $(CFLAGS)
	rm -f $(VALGRIND_OUT)
	valgrind $(CALLGRIND_FLAGS) --callgrind-out-file=$(VALGRIND_OUT) $(OUTPUT)
	sudo kcachegrind $(VALGRIND_OUT)

#-------------------------------------------------

LIBNAME = s_alloc

OS := $(shell uname)

ifeq ($(OS), Linux)
	OSTYPE = Linux
else
	OSTYPE = Windows
endif

RELEASE_ASM = $(REL_DIR)/$(OSTYPE)/lib$(LIBNAME).a
RELEASE_SRC = $(filter-out $(SRC_DIR)/test.c,$(SRC))
RELEASE_OBJS = $(patsubst $(SRC_DIR)/%.c,$(TMP_DIR)/%.o,$(RELEASE_SRC))

release-build:
	@mkdir -p $(REL_DIR)/$(OSTYPE)
	@mkdir -p $(TMP_DIR)
	@$(MAKE) $(RELEASE_OBJS)
	$(AR) rc $(RELEASE_ASM) $(RELEASE_OBJS)
	$(RANLIB) $(RELEASE_ASM)

$(TMP_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ -I$(INC_DIR) $(CFLAGS)
