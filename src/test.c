#include <stdio.h>

#include "static_allocator.h"

int main(){
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

	s_merge();

	return 0;
}
