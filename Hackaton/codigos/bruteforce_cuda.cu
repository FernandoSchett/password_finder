/*
@(#)File:           bruteforce_cuda.cu
@(#)Last changed:   Date: 11/24/2022 21:40:00
@(#)Purpose:        Brute Force Password Breaker with GPU  
@(#)Modified:       Fernando Schettini, Gabriel Mascarenhas, Jadson Nobre
@(#)Usage:
 (*) Hotocompile:   nvcc bruteforce_cuda.cu -o bruteforce_cuda
 (*) Hotoexecute:   ./bruteforce <password> <numbers_of_blocks>
                    ./bruteforce senhatez 30
@(#)Comment:
 (*)   Disponible in: http://forum.guiadohacker.com.br/showthread.php?t=40571
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <math.h>
#include <cuda.h> // importa a biblioteca CUDA
#include <string>
#include <cstring>
#include <assert.h>
using namespace std; //define o namespace padrão

//97 to 122 use only lowercase letters
//65 to 90 use only capital letters
//48 to 57 use only numbers
#define START_CHAR 48
#define END_CHAR 122
#define MAXIMUM_PASSWORD 20

__device__ long long my_pow(long long x, int y){//funcao para calcular potencia, marcada com __device__ para ser executada no device 

  long long res = 1;

  if (y == 0)
    return res;
  else
    return x * my_pow(x, y-1);

}
__device__ int my_strlen(char *s) {
    int sum = 0;
    while (*s++) sum++;
    return sum;
 }

__global__ void bruteForce(char *pass) { //define a função que poderá ser chamada globalmente, como kernel 
  long long j = blockIdx.x*blockDim.x+threadIdx.x;
  int tam = my_strlen(pass);
  int pass_b26[MAXIMUM_PASSWORD];

  long long int pass_decimal = 0;
  int base = END_CHAR-START_CHAR+2;

  for(int i = 0; i < tam; i++){
    pass_b26[i] = (int)pass[i]-START_CHAR+1;
  }
  for(int i = tam - 1; i > -1; i--){
    pass_decimal += (long long int) pass_b26[i]*my_pow(base,i);
  }

  long long int max = my_pow(base,tam);
  char s[MAXIMUM_PASSWORD];

  while(j < max){
      if(j == pass_decimal){
        printf("Encontrou o password!\n");
        int index = 0;
        printf("O número que estamos tentando encontrar (password na base decimal): %lli\n", j);
        while(j > 0){
          s[index++] = START_CHAR + j%base-1;
          j /= base;
        }
        s[index] = '\0';
        printf("Password encontrado: %s\n", s);
        assert(0);
        break;
      }
      j += blockDim.x*gridDim.x; //move o stride loop
  }
    
}

void printTime(char *text, double time){
  FILE *f = fopen("cuda_time.txt", "a");
  fprintf(f, "%s %1.2f\n", text, time);
  fclose(f);
}

int main(int argc, char **argv) {
  int id;
  int numsms;
  cudaGetDevice(&id); //pega o identificador do device
  cudaDeviceGetAttribute(&numsms, cudaDevAttrMultiProcessorCount, id); //pega o número de multiprocessadores do device e o número de SMS
  char *password_gpu; //ponteiro para a variável que será alocada no device
  
  cudaMallocManaged(&password_gpu, MAXIMUM_PASSWORD); //linka ás memórias do device e do host a variável password_gpu
  strcpy(password_gpu, argv[1]); // copia a variavel para duas memorias

  time_t t1, t2;
  double dif;
  int blocos = numsms * atoi(argv[2]); //define o número de blocos
  time(&t1);
  bruteForce<<< blocos, 1024 >>>(password_gpu); //chama o kernel, passando o numero de blocos e threads por bloco
  cudaDeviceSynchronize(); // Espera todas as threads terminarem dentro da GPU
  time(&t2);
  dif = difftime (t2,t1);
  printf("\n%1.2f seconds\n", dif);
  printTime(argv[1], dif);
  cudaFree(password_gpu);
  return 0;

}