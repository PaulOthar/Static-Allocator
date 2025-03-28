SRC_DIR = ./src
INC_DIR = ./include
BIN_DIR = ./bin
REL_DIR = ./release

SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
OUTPUT = $(BIN_DIR)/bin.exe

CFLAGS = -Wall -Wextra -O0 -g3

VALGRIND_OUT = ./valgrind/analysis.out
CALLGRIND_FLAGS = --tool=callgrind --dump-line=yes --dump-instr=yes --collect-jumps=yes --collect-systime=yes --collect-bus=yes --cache-sim=yes --branch-sim=yes --simulate-wb=yes --simulate-hwpref=yes --cacheuse=yes --time-stamp=yes

CC = gcc

all:
	$(CC) $(SRC) -o $(OUTPUT) -I$(INC_DIR) $(CFLAGS) -DSTATIC_ALLOCATOR_DEBUG_MODE
	
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
	OSTYPE = linux
else
	OSTYPE = windows
endif

RELEASE = $(REL_DIR)/$(LIBNAME)_$(OSTYPE).o
RELEASE_ASM = $(REL_DIR)/$(LIBNAME)_$(OSTYPE).a
	
launch:
	$(CC) -c $(SRC_DIR)/$(LIBNAME).c -o $(RELEASE) -I$(INC_DIR)
	ar rcs $(RELEASE_ASM) $(RELEASE)