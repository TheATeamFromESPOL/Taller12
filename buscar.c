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
int main(int argc, char *argv[]){
	if(argc<3){
		return -1;
	}
	char* ruta = argv[2];
	char** palabras;
	int tPalabras=argc-2;
	palabras = (char**)malloc(*sizeof(char*));
	int i;
	for(i = 0;i<tPalabras;i++){
		palabras[i]=argc[i+2];
		printf("%s\n",palbras[i]);
	}
	return 0;
}
