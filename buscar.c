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

	int posFinal = posInicial;
	for(i=linea_inicio;i<=linea_final;i++){
		posFinal += tam_lineas[i];
	}

	//Colocación de puntero en la posición correspondiente
	fseek(fp,posInicial, SEEK_SET);
	
	char *linea = "";
	char *palabra;
	int j;
	pthread_mutex_lock(&lock);
	while((ftell(fp)<=posFinal)&&(fgets(linea,MAX,fp)!=NULL)){
		palabra = strtok(linea,",.!?:;");
		while(palabra!=NULL){
			printf("Palabra: %s",palabra);
			for(j=0;j<tPalabras;j++){
				if(strcmp(palabras[j],palabra)==0){
					num_palabras[j]+=1;
 				}
 				palabra = strtok(NULL,",.!?:;");
			}
		}
	}

	pthread_mutex_unlock(&lock);

	return (void *)0;
}

void *impresionNumPalabras(void * argumento){
	while(1){
		pthread_mutex_lock(&lock);
		int i;
		printf("\nCantidad de veces que aparece cada palabra:\n");
		for(i=0; i<tPalabras;i++){
			printf("%s: %i veces\n",palabras[i],num_palabras[i]);
		}
		pthread_mutex_unlock(&lock);
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
		return -1;
	}

	char* ruta = argv[1];
	int nHilos = atoi(argv[2]);
	printf("Argumentos: %i\n",argc);
	tPalabras=argc-3;
	printf("Palabras: %i\n",tPalabras);

	//Aquí habría que usa una estructura palabrasvecesTDA o sino solamente usar un arreglo global para las palabras y para el num de veces --Opté por los arreglos globales
	/*char** palabras;
	palabras = (char**)malloc(tPalabras*sizeof(char*));
	int num_palabras[tPalabras];*/

	int i;
	for(i = 0 ; i < tPalabras ; i++){
		printf("Palabra a insertar: %s\n",argv[i+3]);
		palabras[i]=argv[i+3];
	}
	/*for(i = 0 ; i < tPalabras ; i++){
		printf("%s\n",palabras[i]);
	}*/
	pthread_t *hilos;
	hilos = (pthread_t*)malloc(nHilos*sizeof(pthread_t));
	//printf("Crea los threads\n");

	int *tam_lineas = (int *)malloc(MAX*sizeof(int));
	//printf("Crea arreglo de tam_lineas\n");
	int nLineas;
	nLineas = numero_lineas(ruta,tam_lineas);//Sale violación de core
	//printf("realiza la funcion numero_lineas\n");
	//printf("numero de lineas del archivo:%i\n",nLineas);
	for(i = 0 ; i < nLineas ; i++){
		printf("linea=%i, valor:%i\n",i+1,tam_lineas[i]);
	}
	int ini,fin,div;
	div = (int)nLineas/nHilos;
	printf("%i",div);
	int j;
	//printf("funciona antes for\n");
	for(j=0;j<nHilos;j++){
		ini=div*i;
		if(i!=(nHilos-1))
			fin=div*(i+1);
		else
			fin=nLineas;
		//Creacion estructura
		estructura *estruc = (estructura *)malloc(sizeof(estructura *));
		estruc->linea_inicio = ini;
		estruc->linea_final = fin;
		estruc->tam_lineas = tam_lineas;
		estruc->ruta = ruta;
		int hilo = pthread_create(&hilos[i], NULL, funcion_hilo,(void *)estruc);
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

	for(j=0;j<nHilos;j++){
		pthread_join(hilos[j],NULL);
	}
	pthread_join(hiloPresentacion,NULL);

	printf("\nFINAL\nCantidad de veces que aparece cada palabra:\n");
	pthread_join(hiloPresentacion,NULL);
	for(i=0; i<tPalabras;i++){
		printf("%s: %i veces\n",palabras[i],num_palabras[i]);
	}

	pthread_mutex_destroy(&lock);
	return 0;
}
