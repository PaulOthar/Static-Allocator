#ifndef MEMORY_DIRECTORY
#define MEMORY_DIRECTORY

#include <stdint.h>

typedef struct _memory_unit{
	char name[16];

	uint32_t meta;
	uint32_t size;
	uint64_t id;

	struct _memory_unit* prev;
	struct _memory_unit* next;
	void* content;
}memory_unit;

/**
 * Initializes important fields in a memory_unit.
 * Usefull for when creating a memory_unit on the stack of a function.
 * @fn void md_init_root(memory_unit*)
 * @param root ptr of the unit to be initalized
 */
void md_init_root(memory_unit* root);

/**
 *
 * @fn void md_alloc_path*(memory_unit*, char*, int)
 * @param root
 * @param path
 * @param size
 */
void* md_alloc_path(memory_unit* root, char* path, int size);

void md_free_unit(memory_unit* unit);

memory_unit* md_fetch_path(memory_unit* root, char* path);

//-----------------------------------------------------------------------------------------

memory_unit* md_header(void* ptr);

void* md_data(memory_unit* unit);

//-----------------------------------------------------------------------------------------

void md_print_tree(memory_unit* root, int identation);

#endif
