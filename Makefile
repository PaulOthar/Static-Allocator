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

CFLAGS += -DSTATIC_ALLOCATOR_DEBUG_MODE
CFLAGS += -DMEMORY_DIRECTORY_DEBUG_MODE

all:
	$(CC) $(SRC) -o $(OUTPUT) -I$(INC_DIR) $(CFLAGS)
	
mcall:
	$(CC) $(SRC) -o $(OUTPUT) -I$(INC_DIR) $(CFLAGS) 
	#make all
	rm -f $(VALGRIND_OUT)
	valgrind $(CALLGRIND_FLAGS) --callgrind-out-file=$(VALGRIND_OUT) $(OUTPUT)
	sudo kcachegrind $(VALGRIND_OUT)
	
#-------------------------------------------------
	
LIBNAME = static_allocator
OS := $(shell uname)
ifeq ($(OS), Linux)
	OSTYPE = Linux
else
	OSTYPE = Windows
endif

RELEASE_OBJ = $(TMP_DIR)/$(LIBNAME).o
RELEASE_ASM = $(REL_DIR)/$(OSTYPE)/lib$(LIBNAME).a
	
release-build:
	@mkdir -p $(TMP_DIR)
	@mkdir -p $(REL_DIR)
	@mkdir -p $(REL_DIR)/$(OSTYPE)
	$(CC) -c $(SRC_DIR)/$(LIBNAME).c -o $(RELEASE_OBJ) -I$(INC_DIR)
	ar rcs $(RELEASE_ASM) $(RELEASE_OBJ)