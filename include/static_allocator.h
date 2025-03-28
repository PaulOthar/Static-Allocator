#ifndef STATIC_ALLOCATOR
#define STATIC_ALLOCATOR

#ifndef STATIC_ALLOCATOR_SIZE
#define STATIC_ALLOCATOR_SIZE (1 << 20)
#endif

extern int avmem_global;
extern int avmem_rover;

/**
 * Allocates the specified number of bytes, with a possible additional cost of 8 bytes for the header.
 * Return null (0) if no big enough space is available.
 * @fn void s_alloc*(int)
 * @param size The size in bytes
 */
void* s_alloc(int size);

/**
 * Liberates the specific pointer's memory, making it available for reallocation, but beware of fragmentation.
 * This function will not automatically merge neighboring free blocks, unless if "STATIC_ALLOCATOR_MERGE_AFTER_FREEING" is defined.
 * This function will not check if it is pointing at a valid header, so tread carefully.
 * @fn void s_free(void*)
 * @param ptr Pointer to be freed
 */
void s_free(void* ptr);

/**
 * Merge will scan and combine neighboring regions of memory, if they are both free.
 * If it reaches the block neighboring the hover, it will effectively resize it to its combined size.
 * @fn void s_merge(void)
 */
void s_merge(void);

#endif
