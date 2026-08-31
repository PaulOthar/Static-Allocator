#include <stdio.h>

#include "static_allocator.h"
char s_pool[1 << 26];

int main(){
	s_init(sizeof(s_pool));

	char* texto = (char*)s_alloc(30);
	for(int i = 0;i<30;i++){
		texto[i] = 0xAE;
	}

	char* texto2 = (char*)s_alloc(10);
	for(int i = 0;i<10;i++){
		texto2[i] = 0xBC;
	}

	s_free(texto2);

	s_free(texto);
	texto = (char*)s_alloc(10);
	for(int i = 0;i<10;i++){
		texto[i] = 0x11;
	}
	s_free(texto);

	int count = s_count();
	void* current = s_first();
	for(int i = 0; i < count; i++){
		printf("-------- size: %d / is_free ? %d --------\n", s_size(current), s_isfree(current));
		current = s_next(current);
	}

	s_merge();

	return 0;
}
