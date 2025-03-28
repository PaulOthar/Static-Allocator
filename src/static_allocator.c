#include "static_allocator.h"

#ifdef STATIC_ALLOCATOR_DEBUG_MODE
#include <stdio.h>
#define STATIC_ALLOCATOR_DEBUG(MESAGE,...) printf("DEBUG: " MESAGE "\n", ##__VA_ARGS__)
#else
#define STATIC_ALLOCATOR_DEBUG(MESAGE, ...)
#endif

#ifndef STATIC_ALLOCATOR_MERGE_AFTER_FREEING
#define STATIC_ALLOCATOR_MERGE_AFTER_FREEING s_merge();
#endif

#define STATIC_ALLOCATOR_TAG_FREE 0
#define STATIC_ALLOCATOR_TAG_USING 1

#define STATIC_ALLOCATOR_STEP_AHEAD(PTR,SIZE) (memheader*)(((void*)(PTR + 1)) + SIZE)

typedef struct memheader{
	int size;
	int tags;
}memheader;

int avmem_global;
int avmem_rover;

static memheader* rover;
static char pool[STATIC_ALLOCATOR_SIZE];
static const int sizemh = sizeof(memheader);

//__attribute__((constructor)) void static_allocator_initialize() {
//	for(int i = 0;i<STATIC_ALLOCATOR_SIZE;i++){ pool[i] = 0xff; }
//	avmem_global = STATIC_ALLOCATOR_SIZE - sizemh;
//	avmem_rover = avmem_global;
//	rover = (memheader*)pool;
//	rover->size = avmem_rover;
//	rover->tags = STATIC_ALLOCATOR_TAG_FREE;
//}

static inline void static_allocator_initialize(){
	avmem_global = STATIC_ALLOCATOR_SIZE - sizemh;
	avmem_rover = avmem_global;
	rover = (memheader*)pool;
	rover->size = avmem_rover;
	rover->tags = STATIC_ALLOCATOR_TAG_FREE;
}

void* s_alloc(int size){
	if(!size){ return 0; }
	if(!rover){ static_allocator_initialize(); }
	STATIC_ALLOCATOR_DEBUG("Attempting memory allocation of %d bytes from a total of %d",size,avmem_global);

	int sizereal = size + sizemh;
	memheader* result = (memheader*)pool;

	if(rover->size >= sizereal){//if the rover has enough space
		result = rover;
		//Step the bytes from the header and the size combined
		rover = (memheader*)(((void*)rover) + sizereal);
		rover->size = result->size - sizereal;
		rover->tags = STATIC_ALLOCATOR_TAG_FREE;

		result->size = size;
		result->tags = STATIC_ALLOCATOR_TAG_USING;

		avmem_global -= sizereal;
		avmem_rover = rover->size;

		STATIC_ALLOCATOR_DEBUG("    Allocating memory from rover: %d + %d (new header), Total = %d",size,sizemh,avmem_global);
		return (void*)(result + 1);
	}

	while(result != rover){//then starting from the beginning until we reach the rover

		//if this is not a free space, or it is free but it doesn't have enough space
		if(result->tags != STATIC_ALLOCATOR_TAG_FREE || result->size < size){
			result = (memheader*)(((void*)(result + 1)) + result->size);
			continue;
		}

		result->tags = STATIC_ALLOCATOR_TAG_USING;
		avmem_global -= result->size;

		STATIC_ALLOCATOR_DEBUG("    Allocating memory from reused header: %d, Total = %d)",result->size,avmem_global);

		//if there isn't any room for more than the header + 1 byte
		if(result->size <= sizereal){
			return (void*)(result + 1);//ship it with the current size (possibly greater)
		}

		//create a new header, with the size of the remaining bytes
		memheader* remainder = (memheader*)(((void*)(result + 1)) + size);
		remainder->size = result->size - sizereal;
		remainder->tags = STATIC_ALLOCATOR_TAG_FREE;
		avmem_global += remainder->size;

		STATIC_ALLOCATOR_DEBUG("    Resizing memory from reused header: %d (reused) - %d (new header) - %d (excess), Total = %d",result->size,sizemh,remainder->size,avmem_global);

		result->size = size;
		return (void*)(result + 1);
	}

	return 0;
}

void s_free(void* ptr){
	memheader* tofree = ((memheader*) ptr) - 1;
	tofree->tags = STATIC_ALLOCATOR_TAG_FREE;
	avmem_global += tofree->size;
	STATIC_ALLOCATOR_DEBUG("Freeing memory: %d, Total: %d",tofree->size,avmem_global);
	STATIC_ALLOCATOR_MERGE_AFTER_FREEING
}

void s_merge(void){
	STATIC_ALLOCATOR_DEBUG("Attempting to merge unused headers, Total: %d",avmem_global);
	memheader* current = (memheader*)pool;
	while(current != rover){
		memheader* next = (memheader*)(((void*)(current + 1)) + current->size);

		if(current->tags != STATIC_ALLOCATOR_TAG_FREE || current->tags != next->tags){
			STATIC_ALLOCATOR_DEBUG("    Could not merge headers: (Size:%d, Tags:%d)<>(Size:%d, Tags:%d), Total: %d",current->size,current->tags,next->size,next->tags,avmem_global);
			current = next;
			continue;
		}

		avmem_global += sizemh;
		STATIC_ALLOCATOR_DEBUG("    Merging headers: (Size:%d)><(Size:%d)=(+%d bytes from header), Total: %d",current->size,next->size,sizemh,avmem_global);
		current->size += next->size + sizemh;

		if(next != rover){
			continue;
		}

		rover = current;
		avmem_rover = rover->size;
		STATIC_ALLOCATOR_DEBUG("    Header merged with rover: Total: %d, Rover: %d",avmem_global,avmem_rover);
		break;
	}
}
