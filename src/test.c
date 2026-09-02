#include <stdio.h>

#include "static_allocator.h"
#include "memory_directory.h"

void basic_alloc_test(){
	char* texto = (char*)s_alloc(30);
	for(int i = 0; i < 30; i++){ texto[i] = 0xAE; }

	char* texto2 = (char*)s_alloc(10);
	for(int i = 0; i < 10; i++){ texto2[i] = 0xBC; }

	s_free(texto2);
	s_free(texto);

	texto = (char*)s_alloc(10);
	for(int i = 0; i < 10; i++){ texto[i] = 0x11; }

	s_free(texto);

	int count = s_count();
	void* current = s_first();
	for(int i = 0; i < count; i++){
		printf("-------- size: %d / is_free ? %d --------\n", s_size(current), s_isfree(current));
		current = s_next(current);
	}

	s_merge();
}

void directory_test(){
	memory_unit root;
	md_init_root(&root);

	md_alloc_path(&root, "silly goofyness", 69);

	char* texto = (char*)md_alloc_path(&root, "documents/text 1", 55);
	for(int i = 0; i < 30; i++){ texto[i] = 0xAE; }

	char* texto2 = (char*)md_alloc_path(&root, "documents/text 2", 30);
	for(int i = 0; i < 10; i++){ texto2[i] = 0xBC; }

	md_alloc_path(&root, "documents/notes/test", 1024);
	md_alloc_path(&root, "documents/notes/test2", 500);

	md_print_tree(&root, 0);

	md_free_unit(md_fetch_path(&root, "documents/notes"));

	md_print_tree(&root, 0);

	s_merge();
}

char s_pool[1024];

int main(){
	s_init(sizeof(s_pool));

	basic_alloc_test();
	printf("----------------------------------------------------------------\n");
	directory_test();

	return 0;
}
