# Static-Allocator - Simple memory allocator library
Static-Allocator is a very simple memory allocator, for building systems with a limited memory budget. Allowing the developer to choose how much memory it will be able to allocate upfront.

## How to use
### Step 1: Include the library in your project

```c
#include "static_allocator.h"
```

### Step 2: Compile and link the library
To link the library you will need to include a few flags:
```bash
# if the library folder is in the root of the project:
-I./Static-Allocator/include -L./Static-Allocator/lib -lstatic_allocator

#if the library folder is in some 'lib' folder, inside the project:
-I./bin/Static-Allocator/include -L./bin/Static-Allocator/lib -lstatic_allocator
```

## Available Functions - `static_allocator`

### 1. **`void s_init(int mem_size)`**

Initializes avmem_global and avmem_rover.
**DOES NOT ALLOCATE SPACE FOR s_pool.**

* **Parameters**: `mem_size` The size in bytes

### 2. **`void* s_alloc(int size)`**

Allocates the specified number of bytes, with a possible additional cost of 8 bytes for the header.
Return null (0) if no big enough space is available.

* **Parameters**: `size` The size in bytes
* **Returns**: A pointer to the allocated memory.

### 3. **`void s_free(void* ptr)`**

Liberates the specific pointer's memory, making it available for reallocation, but beware of fragmentation.
This function will not automatically merge neighboring free blocks, unless if "STATIC_ALLOCATOR_MERGE_AFTER_FREEING" is defined.
This function will not check if it is pointing at a valid header, so tread carefully.

* **Parameters**: `ptr` Pointer to be freed

### 4. **`void s_merge(void)`**

Merge will scan and combine neighboring regions of memory, if they are both free.
If it reaches the block neighboring the hover, it will effectively resize it to its combined size.

### 5. **`int s_merge_at(void* ptr)`**

Attempts to merge a specific block to its right neighbor.

* **Parameters**: `ptr` To merge
* **Returns**: 1 if it could merge, 0 if could not.

### 6. **`int s_within(void* ptr)`**

Verifies if the pointer is within the memory pool range.

* **Parameters**: `ptr` Pointer to be verified
* **Returns**: 1 if it is, 0 if it is not.

### 7. **`int s_validate(void* ptr)`**

Verifies if the pointer has a known header.

* **Parameters**: `ptr` Pointer to be verified
* **Returns**: 1 if it is, 0 if it is not.

### 8. **`int s_size(void* ptr)`**

Gets the size of this allocated block declared in the header.

* **Parameters**: `ptr` Pointer to be verified
* **Returns**: size declared in the header

### 9. **`int s_isfree(void* ptr)`**

Verifies the block's header, if it is declared as used or free.

* **Parameters**: `ptr` Pointer to be verified
* **Returns**: 1 if it is free, 0 if it is being used

### 10. **`int s_count(void)`**

Counts how many blocks there are. (free or not)

* **Returns**: how many exist.

### 11. **`void* s_first(void)`**

Returns the address of the first block. (returns 0 if there is no block)

* **Returns**: a pointer to the first block. (or 0 if there is no block)

### 12. **`void* s_next(void* ptr)`**

Returns the address of the next block. (returns 0 if there is no block ahead)

* **Parameters**: `ptr` Current pointer
* **Returns**: a pointer to the next blocks. (or 0 if it reached the end)

### 13. **`int s_free_safe(void* ptr)`**

Verifies if the specified block is valid before freeing, then free's it, if applicable.

* **Parameters**: `ptr` Pointer to be verified / freed
* **Returns**: 1 if it was freed successifully. 0 if not.

## Available Functions - `memory_directory`

### 1. **`void md_init_root(memory_unit* root)`**

Initializes important fields in a memory_unit.
Usefull for when creating a memory_unit on the stack.

* **Parameters**: `root` Pointer of the unit to be initalized

### 2. **`void* md_alloc_path(memory_unit* root, char* path, int size)`**

Allocates a new unit and attaches it to the specified root, following the specific path.
Creates new directories if the specified path does not exist yet.
Each segment must be separated by a forward slash: '/'

* **Parameters**: `root` Pointer of the main unit; `path` Sequence of directory names separated by a forward slash '/'; `size` Size of the allocated size of the final unit
* **Returns**: a pointer to the allocated memory

### 3. **`void md_free_unit(memory_unit* unit)`**

Liberates the specific pointer's (and its children) memory, making it available for reallocation. (prone to fragmentation)
If you free a directory, every subdirectory and 'file' will also be freed.

* **Parameters**: `unit` Pointer of the unit to be freed

### 4. **`memory_unit* md_fetch_path(memory_unit* root, char* path)`**

Finds and returns a `memory_unit` on the specified path.

* **Parameters**: `root` Pointer to the main unit; `path` Sequence of directory names separated by a forward slash '/'
* **Returns**: a pointer to the unit found, or null (0) if none were found

### 5. **`memory_unit* md_header(void* ptr)`**

Reads the header of the specified block.
WARNING: if this block was not allocated by the directory system, it may cause segfault.

* **Parameters**: `ptr` Pointer to block of memory
* **Returns**: a pointer to the header of the specified block.

### 6. **`void* md_data(memory_unit* unit)`**

Reads the block of the specified header.

* **Parameters**: `unit` Pointer to the header
* **Returns**: a pointer to the block of the specified header

## Important Variables and Macros

### Global Variables:

- **`static_memory_size`**: The initial and total size of the memory pool.
- **`avmem_global`**: The general available memory size, including fragmented.
- **`avmem_rover`**: The remaining size of the rover, i.e. unfragmented memory size.
- **`s_pool`**: The main pool of memory.

### Compiler Macros

Macros that once used during compilation, will produce specific effects on the code.

### 1. **`STATIC_ALLOCATOR_DEBUG_MODE`**
If defined, will print a message to the console, whenever a memory related function is called.

### 2. **`STATIC_ALLOCATOR_MERGE_AFTER_FREEING`**
If defined, will attempt to merge ALL blocks of memory every time `free(void* ptr)` is called.

### 3. **`STATIC_ALLOCATOR_SIZE`**
Defines the size of the memory pool in bytes, not allowing `s_pool` to be manually declared.

### 4. **`MEMORY_DIRECTORY_DEBUG_MODE`**
If defined, will print a message to the console, whenever a memory directory related function is called.

### 5. **`MEMORY_DIRECTORY_SYSTEM_ALLOCATOR`**
If defined, will force the compiler to use the system allocator (`<stdlib.h> malloc, free`).

### User Macros

Macros that can be used anytime, anywhere.

### 1. **`STATIC_ALLOCATOR_INITIALIZE`**
Initializes the memory system.

## License

This library is licensed under the **MIT License**. You can read more about it at [https://opensource.org/licenses/MIT](https://opensource.org/licenses/MIT).
