#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000000
int numero_lineas(char *ruta, int *tam_lineas);

int numero_lineas(char *ruta, int *tam_lineas){
	if(ruta!=NULL){
		FILE* ar = fopen(ruta,"r");
		int lineas = 0;
		int tam_linea;
		while(!feof(ar)){
			tam_linea++;
			char c = getc(ar);
			if(c=='\n'){
				if(tam_lineas!=NULL){
					tam_lineas[lineas] = tam_linea;
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
int main(int argc, char *argv[]){
	if(argc<4){
		return -1;
	}
	char* ruta = argv[1];
	char** palabras;
	int tPalabras=argc-3;
	palabras = (char**)malloc(tPalabras*sizeof(char*));
	int i;
	for(i = 0 ; i < tPalabras ; i++){
		palabras[i]=argv[i+3];
	}
}
