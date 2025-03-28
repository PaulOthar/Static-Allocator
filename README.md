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

## Available Functions

### 1. **`void* s_alloc(int size)`**

Allocates the specified number of bytes, with a possible additional cost of 8 bytes for the header.
Return null (0) if no big enough space is available.

- **Parameters**: `size` — The size in bytes.
- **Returns**: A pointer to the allocated memory.

### 2. **`void s_free(void* ptr)`**

Liberates the specific pointer's memory, making it available for reallocation, but beware of fragmentation.
This function will not automatically merge neighboring free blocks, unless if `STATIC_ALLOCATOR_MERGE_AFTER_FREEING` is defined.
This function will not check if it is pointing at a valid header, so tread carefully.

- **Parameters**: `ptr` — Pointer to be freed

### 3. **`void s_merge(void)`**

Merge will scan and combine neighboring regions of memory, if they are both free.
If it reaches the block neighboring the hover, it will effectively resize it to its combined size.

## Important Variables and Macros

### Global Variables:

- **`avmem_global`**: The general available memory size, including fragmented.
- **`avmem_rover`**: The remaining size of the rover, i.e. unfragmented memory size.

### Macros

There are several macros that if defined, can:
* Hint how the memory is operating.
* Merge blocks of memory upon freeing a pointer
* Resize the memory pool. 

### 1. **`STATIC_ALLOCATOR_DEBUG_MODE`**
If defined, will print a message to the console, whenever a memory related function is called (alloc, free, merge).

### 2. **`STATIC_ALLOCATOR_MERGE_AFTER_FREEING`**
If defined, will attempt to merge neighboring blocks of memory every time `free(void* ptr)` is called.

### 3. **`STATIC_ALLOCATOR_SIZE`**
Defines the size of the memory pool in bytes.
Just keep in mind that allocating memory comes with an aditional cost of the header, that is usually 8 bytes; Therefore it will start at n - 8 bytes.

- **Customizable**: Adjust it as needed to match your budget.

## License

This library is licensed under the **MIT License**. You can read more about it at [https://opensource.org/licenses/MIT](https://opensource.org/licenses/MIT).
