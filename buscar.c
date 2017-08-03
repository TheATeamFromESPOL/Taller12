#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000000
int numero_lineas();

int numero_lineas(){
	if(ruta!=NULL){
		File* ar = fopen(ruta,"r");
		int lineas = 0;
		int tam_linea;
		while(!feof(ar)){
			tam_linea++;
			char c = getc(ar);
			if(c=='\n'){
				if(tam_lineas!=NULL){
					tam_lineas[lineas] = tam_lineas;
				}
				lineas++;
				tam_lineas = 0;
			}
		}
		fclose(ar);
		return lineas;
	}
	return -1;
}
