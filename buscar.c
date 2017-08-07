#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 1000000

pthread_mutex_t lock;
int num_palabras[MAX];
char *palabras[MAX];
int tPalabras;

int palabraEnArreglo(char **arreglo,char *palabra);
int numero_lineas(char *ruta, int *tam_lineas);
void *funcion_hilo(void *estruc);
void *impresionNumPalabras(void *estruc);

typedef struct mi_estructuraTDA{
	int linea_inicio;
	int linea_final;
	int *tam_lineas;
	char *ruta;
}estructura;

int numero_lineas(char *ruta, int *tam_lineas){
	if(ruta!=NULL){
		FILE* ar = fopen(ruta,"r");
		int lineas = 0;
		int tam_linea = 0;
		while(!feof(ar)){
			tam_linea++;
			char c = getc(ar);
			if(c=='\n'){
				if(tam_lineas!=NULL){
					tam_lineas[lineas] = tam_linea;
				}
				lineas++;
				tam_linea = 0;
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
	int *tam_lineas = datos->tam_lineas;
	char *ruta = datos->ruta;

	//Open de archivo
	FILE *fp = fopen(ruta,"r");

	int posInicial = 0;
	int i;
	for(i=0;i<linea_inicio;i++){
		posInicial += tam_lineas[i];
	}

	int j;
	int posFinal = posInicial;
	for(j=linea_inicio;j<=linea_final;j++){
		posFinal += tam_lineas[j];
	}

	//Colocación de puntero en la posición correspondiente
	fseek(fp,posInicial, SEEK_SET);
	
	/*
	while((ftell(fp)<=posFinal)&&(fgets(linea,MAX,fp)!=NULL)){
		palabra = strtok(linea," ,.!?:;");
		while(palabra!=NULL){
			printf("Palabra: %s",palabra);
			for(j=0;j<tPalabras;j++){
				pthread_mutex_lock(&lock);
				if(strcmp(palabras[j],palabra)==0){
					num_palabras[j]+=1;
 				}
				pthread_mutex_unlock(&lock);
 				palabra = strtok(NULL," ,.!?:;");
			}
		}
	}
	*/

	pthread_mutex_lock(&lock);
	char *linea = (char *)malloc(sizeof(char)*MAX);
	char *palabra = "";
	int k;
	for(k = linea_inicio;k < linea_final;k++){
		fgets(linea,MAX,fp);
		palabra = strtok(linea," \n\e\t\\\?\f\'\"\v\b\r!^><·$%&/,()=º¡¢£¤¥¦§¨©ª«®¯|@#½¬-_ç`+*[]{}ḉç¿,.!?:;");
		while(palabra!=NULL){
			//printf("Palabra: %s\n",palabra);
			int l;
			for(l=0;l<tPalabras;l++){
				//printf("Palabras[%i]: %s\n",l,palabras[l]);
				if(strcmp(palabras[l],palabra)==0){
					num_palabras[l] += 1;
 				}
 				palabra = strtok(NULL," \n\e\t\\\?\f\'\"\v\b\r!^><·$%&/,()=º¡¢£¤¥¦§¨©ª«®¯|@#½¬-_ç`+*[]{}ḉç¿,.!?:;");
			}
		}
	}
	pthread_mutex_unlock(&lock);
	return (void *)0;
}

void *impresionNumPalabras(void * argumento){
	while(1){
		//pthread_mutex_lock(&lock);
		int i;
		printf("\nCantidad de veces que aparece cada palabra:\n");
		for(i=0; i<tPalabras;i++){
			printf("%s: %i veces\n",palabras[i],num_palabras[i]);
		}
		//pthread_mutex_unlock(&lock);
		sleep(1);
	}
	return (void *)0;
}

int main(int argc, char *argv[]){

	if (pthread_mutex_init(&lock, NULL) != 0){
        	printf("\n mutex init failed\n");
        	return 1;
    	}

	if(argc<4){
		printf("Cantidad de argumentos incorrecta");
		return -1;
	}

	char* ruta = argv[1];
	int nHilos = atoi(argv[2]);
	tPalabras=argc-3;

	int i;
	for(i = 0 ; i < tPalabras ; i++){
		palabras[i]=argv[i+3];
	}
	
	pthread_t *hilos;
	hilos = (pthread_t*)malloc(nHilos*sizeof(pthread_t));

	int *tam_lineas = (int *)malloc(MAX*sizeof(int));
	int nLineas;
	nLineas = numero_lineas(ruta,tam_lineas);

	int ini,fin,div;
	div = (int)nLineas/nHilos;
	int j;
	//printf("funciona antes for\n");
	for(j=0;j<nHilos;j++){
		ini=div*j;
		if(j!=(nHilos-1))
			fin=div*(j+1);
		else
			fin=nLineas;
		//printf("Linea Inicio: %i   Linea Fin: %i",ini,fin);
		//Creacion estructura
		//printf("creacion estructura\n");
		estructura *estruc = (estructura *)malloc(sizeof(estructura *));
		estruc->linea_inicio = ini;
		estruc->linea_final = fin;
		estruc->tam_lineas = tam_lineas;
		estruc->ruta = ruta;
		//printf("Se crea la estructura\n");
		int hilo = pthread_create(&hilos[j], NULL, funcion_hilo,(void *)estruc);
		if(hilo<0){
			printf("Error creando hilo de conteo.");
			return 0;
		}
	}
	//printf("salio del for\n");
	
	pthread_t presentador;
	int hiloPresentacion = pthread_create(&presentador, NULL, impresionNumPalabras, NULL);
	if(hiloPresentacion<0){
		printf("Error creando hilo de presentación.");
		return 0;
	}
	
	/*printf("Se hace los joins\n");
	int hue;
	for(hue=0;hue<nHilos;hue++){
		pthread_join(hilos[hue],NULL);
	}*/
	//pthread_join(presentador,NULL);

	/*printf("\nFINAL\nCantidad de veces que aparece cada palabra:\n");
	pthread_join(hiloPresentacion,NULL);
	for(i=0; i<tPalabras;i++){
		printf("%s: %i veces\n",palabras[i],num_palabras[i]);
	}*/

	pthread_mutex_destroy(&lock);
	return 0;
}
