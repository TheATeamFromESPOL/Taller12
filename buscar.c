#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000000

int palabraEnArreglo(char **arreglo,char *palabra);
int numero_lineas(char *ruta, int *tam_lineas);
void *funcion_hilo(void *estruc);
void *impresionNumPalabras(void *estruc);

typedef struct mi_estructuraTDA{
	int linea_inicio;
	int linea_final;
	int tPalabras;
	int *tam_lineas;
	int *num_palabras;
	char *ruta;
	char **palabras;
}estructura;

typedef struct palabrasVecesTDA{
	char **palabras;
	int *num_palabras;
}

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
	char **palabras = datos->palabras;

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
				temp = *(palabras+j);
				if(strcmp(temp,palabra)==0){
					*(num_palabras+j)+=1;
				}
				palabra = strtok(NULL,",.!?:;");
			}
		}
	}

	return (void *)suma;
}

void *impresionNumPalabras(void *estruc){

}

int main(int argc, char *argv[]){
	if(argc<4){
		return -1;
	}
	char* ruta = argv[1];
	int nHilos = atoi(argv[2]); 
	int tPalabras=argc-3;
	//Aquí habría que usa una estructura palabrasvecesTDA o sino solamente usar un arreglo global para las palabras y para el num de veces
	char** palabras;
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
	
	int ini,fin,div;
	div = (int)nLineas/nHilos;
	int j;
	for(j=0;j<nHilos;i++){
		ini=div*i;
		if(i!=(nHilos-1))
			fin=div*(i+1);
		else
			fin=nLineas;
		//Creacion estructura
		estructura *estruc = (estructura *)malloc(sizeof(estructura *));
		estruc->linea_inicio = ini;
		estruc->linea_final = fin;
		estruc->tPalabras = tPalabras;
		estruc->tam_lineas = tam_lineas;
		estruc->num_palabras = num_palabras;
		estruc->ruta = ruta;
		estruc->palabras = palabras;
	}

	return 0;
}
