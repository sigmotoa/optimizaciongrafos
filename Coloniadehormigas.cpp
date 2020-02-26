	/* COLONIA DE HORMIGAS: Aplicación al problema del Clique maximo
	sigmotoa
	Ciencias Basicas
	MITC
	Sexto semestre
	26 de febrero de 2020*/
	#include <stdio.h>
	#include <stdlib.h>
	#include <cstdlib>
	#include <iostream>
	#include <stack>
	#include <math.h>
	#include <time.h>
	#include <conio.h>

	using namespace std;

	int matriz[100][100],n,m, iteraciones,candidatos[100]; //Matriz para el ingreso del grafo, n=vertices, m=hormigas, iteraciones=sitio mas lejano
	float beta, alpha, tau0, feromonas[100][100], evaporacion;//Parametros del algoritmo
	int cliquemaximal[100],  cliquemaximalmejor[100], evaluacion, grados[100], valormejor=0;

	int candidatos_novacio(void);
	void lectura_archivo(void);
	void construir_solucion(int j);
	void actualizar_mejor(int j, int i);
	void actualizar_feromonas(void);
	void actualizar_evaporacion(void);

	int main(int argc, char *argv[]){
		srand(time(NULL));
		lectura_archivo( );
		for (int i=0; i < iteraciones; i++){
			for (int j=0; j < m; j++){
				construir_solucion(j);
				actualizar_mejor(j, i);
			}
			actualizar_feromonas();
			actualizar_evaporacion();
		}

		system("Pause");
	}

	void lectura_archivo( ){
	    char archivo[250];
		FILE *grafo;
		printf("Archivo datos del problema CLIQUE MAX: ");
		scanf("%s", &archivo);
		grafo=fopen(archivo,"r");
		if (grafo == NULL){
			printf("\nError de apertura del archivo.");
			exit(0);
	    }

	    fscanf(grafo, "%d", &n); // número de vertices
	    fscanf(grafo, "%d", &m); // número de hormigas
	    fscanf(grafo, "%f", &beta); // Beta
	    fscanf(grafo, "%f", &alpha); // Alfa
	    fscanf(grafo, "%f", &tau0); // Tau0
	    fscanf(grafo, "%f", &evaporacion);//Evaporacion
	    fscanf(grafo,  "%d", &iteraciones); // iteraciones ciclo externo
		for(int i=0; i < n; i++){   // matriz de adyacencias
			for(int j=0; j < n; j++) {
				fscanf(grafo,"%d", &matriz[i][j]);
			}
		}
		fclose(grafo);
		for (int i= 0 ; i < n; i++)
			for (int j=0; j < n; j++)
				feromonas[i][j]=tau0;
		// tomar la cliquemaximal trivial como primera y evaluarla y ponerla
		// en valormejor
		//for(int i=0; i< n; i++)
		//	permutmejor[i]=i;
		//valormejor=0;
		//for (int i=0; i < (n-1); i++){
		//	valormejor=valormejor+matriz[permutmejor[i]][permutmejor[i+1]];
		//}
		//valormejor=valormejor+matriz[permutmejor[n-1]][permutmejor[0]];

	for(int i=0; i < n; i++){   // matriz de adyacensia
				grados[i]=0;
			for(int j=0; j < n; j++) {
				if (matriz[i][j]==1) grados[i]=grados[i]+1;
				printf("%d ", matriz[i][j]);
			}

		}
		for(int i=0; i < n; i++){   // imprimir matriz de grafo
		for(int j=0; j < n; j++) {
	   printf("%d ", matriz[i][j]);
			}
			printf("\n");
		}


	}
	void construir_solucion(int j /* hormiga j-esima */) {
		int contcand,  posmayor;
		float probij[100], suma, mayorprob;


		//for (int i=0;  i < n; i++)
		//	candidatos[i]=1;
		cliquemaximal[0]=rand()%n;
		
		for(int i =0; i<n; i++)
		{
			candidatos[i]=matriz[cliquemaximal[0]][i];
		}
		contcand=0;
		
		
		while (candidatos_novacio()==0) {//Validar los 1 en el vector de candidatos
			// construye la cliquemaximal y la probabilidad del siguiente vertice
			suma=0;
			for (int i=0; i < n; i++) {// numerador y denominador de PROB
				if (candidatos[i]==1) {
					probij[i] = // numerador
				//	pow((float) 1/matriz[cliquemaximal[contcand-1]][i], alpha)//cuidado, revisar  por 						cliquemaximal
					//	*pow(feromonas[cliquemaximal[contcand-1]][i], beta);//probabilidad de la ciudad siguiente 						revisar apuntes
					pow(grados[contcand-1], alpha)
					*pow(feromonas[cliquemaximal[contcand]][i], beta);
					suma = suma + // denominador
					probij[i];
					// pow((float) 1/matriz[cliquemaximal[contcand-1]][i], alpha)
					//	*pow(feromonas[cliquemaximal[contcand-1]][i], beta);
				}
				else {
					probij[i] = 0;	//numerador es 0, vertice no se elige
				}
			} // fin de ciclo i numerador y denominador
			// dividir el numerador entre el denominador
			// ademas tenemos que calcular el vertice de mayor probabilidad
			mayorprob=0; posmayor=0;
			printf("PROBABILIDADES: ");
			for (int i=0; i < n; i++) {
				probij[i]=probij[i]/suma;
				printf("%f ", probij[i]);
				if (probij[i] > mayorprob) {
					mayorprob=probij[i]; posmayor=i;
				}
			}
			printf("\n");
			contcand++;
			cliquemaximal[contcand]=posmayor;
			
			for (int i =0; i<n;i++)
			{
				if(candidatos[i]+matriz[cliquemaximal[contcand]][i]==2)
				candidatos[i]=1;
				else candidatos[i]=0;
				
		}
			candidatos[posmayor] = 0;
		} // fin de ciclo de contcand construir la cliquemaximal
		//imprimir solucion encontrada por hormiga j
		printf("La hormiga %d construye la solucion: ", j);
		for (int i =0; i < n; i++)
			printf("%d ", cliquemaximal [i]);
		printf("\n");
		// evaluando la solucion de la hormiga j-esima para clique maximo
		evaluacion=contcand;
		for (int i=0; i < n -1; i++)	{
			evaluacion=evaluacion+matriz[cliquemaximal[i]][cliquemaximal[i+1]];
		}
		evaluacion=evaluacion+matriz[cliquemaximal[n-1]][cliquemaximal[0]];
		printf("Evaluación de la solución: %d hormiga: %d", evaluacion, j);
		printf("\n");
	} // fin construir_solucion()
	void actualizar_mejor(int j, int i) {
	  if (evaluacion > valormejor) {
	  	valormejor=evaluacion;
	  	printf("Mejor solucion hasta el momento, i= %d j = %d\n", i,j);
	  	for (int k=0; k < n; k++){
	  	cliquemaximalmejor[k]	= cliquemaximal[k];
	  		printf("%d ", cliquemaximalmejor[k]);
	  	}
	  	printf("%d", valormejor);
	  	printf("\n");
	  }
	}
	void actualizar_feromonas(void){
	
	//usar criterio de ciclo de los vertices del clique maximal
		for (int i=0; i < (n-1); i++) {
			feromonas[cliquemaximalmejor[i]][cliquemaximalmejor[i+1]]=
				valormejor;
		}
		feromonas[cliquemaximalmejor[n-1]][cliquemaximalmejor[0]] =
			valormejor;
	}
	void actualizar_evaporacion(void) {
		for (int i=0; i< n; i++){
			for (int j=0; j < n; j++) {
				feromonas[i][j]=(1-evaporacion)*feromonas[i][j];
			}
		}

	}

int candidatos_novacio(void)
{
for(int i=0;i<n;i++)
{
	if(candidatos[i]!=0) return 0;
}
return 1;
}


