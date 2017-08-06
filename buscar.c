#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000000

int palabraEnArreglo(char **arreglo,char *palabra);
int numero_lineas(char *ruta, int *tam_lineas);
void * funcion_hilo(void *estruc);

typedef struct mi_estructuraTDA{
	int linea_inicio;
	int linea_final;
	int tPalabras;
	int *tam_lineas;
	int *num_palabras;
	char *ruta;
	char **palabras;
}estructura;

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

void * funcion_hilo(void *estruc){
	
	estructura *datos= (estructura *)estruc;
	
	int linea_inicio = datos->linea_inicio;
	int linea_final = datos->linea_final;
	int tPalabras = datos->tPalabras;
	int *tam_lineas = datos->tam_lineas;
	int *num_palabras = datos->num_palabras;
	char *ruta = datos->ruta;
	char **palabras = datos->*palabras;

	//Open de archivo
	FILE *fp = fopen(ruta,"r");

	int posInicial = 0;
	int i;
	for(i=0;i<linea_inicio;i++){
		posInicial += tam_lineas[i];
	}

	int posFinal = posInicial;
	for(i=linea_inicio;i<=linea_final;i++){
		posFinal += tam_lineas[i];
	}

	//Colocación de puntero en la posición correspondiente
	fseek(fp,posInicial, SEEK_SET);
	
	char *linea;
	char *palabra;
	char *temp;
	int j;
	while((ftell(fp)<=posFinal)&&(fgets(linea,MAX,fp)!=NULL)){
		palabra = strtok(linea,",.!?:;");
		while(palabra!=NULL){
			for(j=0;j<tPalabras;j++){
				temp = palabras+j;
				if(strcmp(temp,palabra)==0){
					(num_palabras+j)+=1;
				}
				palabra = strtok(NULL,",.!?:;");
			}
		}
	}

	return (void *)suma;
}

int main(int argc, char *argv[]){
	if(argc<4){
		return -1;
	}
	char* ruta = argv[1];
	int nHilos = atoi(argv[2]); 
	char** palabras;
	int tPalabras=argc-3;
	palabras = (char**)malloc(tPalabras*sizeof(char*));
	int num_palabras[tPalabras];
	int i;
	for(i = 0 ; i < tPalabras ; i++){
		palabras[i]=argv[i+3];
	}
	pthread_t *hilos;
	hilos = (pthread_t*)malloc(nHilos*sizeof(pthread_t));
	
	int nLineas;
	int *tam_lineas;
	nLineas = numero_lineas(ruta,tam_lineas);


	return 0;
}
