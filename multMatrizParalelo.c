#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/sysinfo.h>

int tam_matrizA;
int tam_matrizB;
int tam_matrizC;
char v1[50];
char v2[50];
char v3[50];
FILE *arquivo1;
FILE *arquivo2;
FILE *arquivo3;
float **A;
float **B;
float **C;

void escreverArquivo(float **C);
void *multiplica_matriz();

void lerArquivo(){
		
	//Cria ponteiros para os arquivos de texto
	arquivo1 = fopen(v1, "r");
	arquivo2 = fopen(v2, "r");
	
	//Testa se os arquivos de texto existem
	if(arquivo1 == NULL || arquivo2 == NULL){
		printf("Erro!\n\n");
		return;
	}
	//Lê os tamanhos das matrizes (primeira linha de cada arquivo de texto) e salva nas variáveis
	fscanf(arquivo1,"%d\n", &tam_matrizA);
	fscanf(arquivo2,"%d\n", &tam_matrizB);
	
	tam_matrizC = tam_matrizA;
	int i, j=0;
	A = (float**)malloc(tam_matrizA * sizeof(float*)); //Aloca um vetor de ponteiros matriz A
	B = (float**)malloc(tam_matrizA * sizeof(float*)); //Aloca um vetor de ponteiros matriz B
	C = (float**)malloc(tam_matrizA * sizeof(float*)); //Aloca um vetor de ponteiros matriz C
	
	//Percorre as linhas do vetor de ponteiros
	for (i = 0; i < tam_matrizA; i++){ 
		A[i] = (float*) malloc(tam_matrizA * sizeof(float)); //Aloca um vetor de float para cada posição do vetor de ponteiros.
		B[i] = (float*) malloc(tam_matrizB * sizeof(float)); //Aloca um vetor de float para cada posição do vetor de ponteiros.
		C[i] = (float*) malloc(tam_matrizC * sizeof(float)); //Aloca um vetor de float para cada posição do vetor de ponteiros.
	}
      
	//Lendo as matrizes dos arquivos
	for(i=0; i<tam_matrizA; i++){
		for(j=0; j<tam_matrizA; j++){
			if(j == tam_matrizA-1){
				fscanf(arquivo1, "%f\n", &A[i][j]);
				fscanf(arquivo2, "%f\n", &B[i][j]);
			}else{
				fscanf(arquivo1, "%f:", &A[i][j]);
				fscanf(arquivo2, "%f:", &B[i][j]);
			}
		}
	}
	//numProc recebe o número de processadores atualmente disponíveis no sistema
	int numProc = get_nprocs();
	//Vetor de threads
	pthread_t thread[numProc];
	
	//Cria um vetor que guarda a posição inicial e final que cada thread vai receber da matriz
	int vetPosicoes[2*numProc];
	vetPosicoes[0] = 0;
	//O número de linhas por thread é o tamanho da matriz(igual ao número de linhas) dividido pelo número de threads
	int numLinhasThreads = tam_matrizA/numProc;
	vetPosicoes[1] = numLinhasThreads;
	//Esse for vai atribuir o valor corresponde dos limites de cada thread para cada posição do vetor de posições
	for(i=2; i < 2 * numProc; i++){
		vetPosicoes[i] = vetPosicoes[i-2]+ numLinhasThreads;
	}
	
	printf("Calculando o produto de matrizes...\n");
	j=0;
	for (i = 0; i < numProc; i++){
		//Cria as threads passando (endereco da thread,...,funcao, parametros)
		pthread_create(&thread[i], NULL, multiplica_matriz, &vetPosicoes[j]);		
		j+=2;
	}
	//Espera todas as threads terminarem a execução para depois salvar no arquivo
	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);
	pthread_join(thread[2],NULL);
	pthread_join(thread[3],NULL);
	
	//Chama função para salvar a matriz resultante no arquivo
	escreverArquivo(C);
	
	//Fecha os arquivos de texto
	fclose(arquivo1);
	fclose(arquivo2);
}

void escreverArquivo(float **C){
    //Abre o arquivo de texto para escrever no arquivo
    arquivo3 = fopen(v3, "w");
    
    //Testa se foi possível abrir/criar arquivo de texto
    if(arquivo3 == NULL){
		printf("Não foi possível abrir o arquivo! \n\n");
		return;
	}
    //Salva na primeira linha do arquivo de texto o tamanho da matriz
    fprintf(arquivo3,"%d\n", tam_matrizC);
    
    //Escreve a matriz no arquivo de texto
    int i, j;
    for(i = 0; i < tam_matrizC; i++){
		for(j = 0; j < tam_matrizC; j++){
			if(j == tam_matrizC-1){
				fprintf(arquivo3, "%.1f\n", C[i][j]);
			}else{
				fprintf(arquivo3, "%.1f:", C[i][j]);
			}
		}
    }
    
    printf("Matriz resultante salva em %s \n", v3);
    
    //Fecha o arquivo de texto
    fclose(arquivo3);
}

void *multiplica_matriz(int *arg){
	//Passa os valores dos endereços para as variáveis
	int min = arg[0];
	int max = arg[1];
	
	int i, j, k;
	
	//Realiza a multiplicação das matrizes
	for(i=min; i<max; i++){
		for(j=0; j<tam_matrizA; j++){
			C[i][j] = 0;
			for(k=0; k<tam_matrizB; k++){
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return 0;
}   

int main(int argc, char *argv[]){
   
	//executa o que vc quer
	if(argc != 4){
		printf("\nFalha nos parametros!\n\n");
		return -1;
	}
	strcpy(v1, argv[1]);
	strcpy(v2, argv[2]);
	strcpy(v3, argv[3]);
	
	//Chama a funcao que lê o arquivo
	lerArquivo();
	
	return 0;
}
