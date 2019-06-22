#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>


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
int linha;
int coluna;

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
	// float A[tam_matrizA][tam_matrizA], B[tam_matrizB][tam_matrizB];
	int j;
	long int i;
	A = (float**)malloc(tam_matrizA* sizeof(float*)); //Aloca um vetor de ponteiros
	B = (float**)malloc(tam_matrizA* sizeof(float*)); //Aloca um vetor de ponteiros
	C = (float**)malloc(tam_matrizA* sizeof(float*)); //Aloca um vetor de ponteiros
	
	for (i = 0; i < tam_matrizA; i++){ //Percorre as linhas do vetor de ponteiros
		A[i] = (float*) malloc(tam_matrizA * sizeof(float)); //Aloca um vetor de float para cada posição do vetor de ponteiros.
		B[i] = (float*) malloc(tam_matrizB * sizeof(float)); //Aloca um vetor de float para cada posição do vetor de ponteiros.
		C[i] = (float*) malloc(tam_matrizC * sizeof(float)); //Aloca um vetor de float para cada posição do vetor de ponteiros.
     
	}
	
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
	
	printf("Calculando o produto de matrizes...\n");
	for (i = 0; i < tam_matrizA*tam_matrizA; i++){
		multiplica_matriz();
		coluna++;
		if(coluna%tam_matrizA == 0){
			coluna = 0;
			linha++;
		}
	}
	
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

void *multiplica_matriz(){
	int i;
	C[linha][coluna] = 0;
	
	for(i=0; i <tam_matrizA; i++){
		C[linha][coluna] += A[linha][i] * B[i][coluna];
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
