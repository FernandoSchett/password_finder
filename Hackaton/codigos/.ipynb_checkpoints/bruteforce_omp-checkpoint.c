/*
@(#)File:           bruteforce_omp.c
@(#)Last changed:   Date: 11/24/2022 21:40:00
@(#)Purpose:        Brute Force Password Breaker with MultiThreading  
@(#)Modified:       Fernando Schettini, Gabriel Mascarenhas, Jadson Nobre
@(#)Usage:
 (*) Hotocompile:   gcc bruteforce_omp.c -o bruteforce_omp -fopenmp -O3 -std=c99
 (*) Hotoexecute:   ./bruteforce <password> <threadnumber>
                    ./bruteforce senhatez 8 
@(#)Comment:
 (*)   Disponible in: http://forum.guiadohacker.com.br/showthread.php?t=40571
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>

//97 to 122 use only lowercase letters
//65 to 90 use only capital letters
//48 to 57 use only numbers
#define START_CHAR 48
#define END_CHAR 122
#define MAXIMUM_PASSWORD 20

long long my_pow(long long x, int y){
  long long res = 1;
  if (y==0)
    return res;
  else
    return x*my_pow(x,y-1);
}

void bruteForce(char *pass) {
  time_t t1, t2;
  double dif;
  t1 = time(&t1);
  int size;
  size = strlen(pass);
  int pass_b26[size];
  long long int j;
  long long int pass_decimal = 0;
  int base = END_CHAR-START_CHAR+2;

  printf("Estamos tentando quebrar: %s\n", pass);

  for(int i = 0; i < size; i++){
    pass_b26[i] = (int)pass[i]-START_CHAR+1; 
  }
  for(int i = size - 1; i > -1; i--){
    pass_decimal += (long long int) pass_b26[i]*my_pow(base,i);
  }

  long long int max = my_pow(base,size);
  char s[size];
  int achou = 0;
  
  #pragma omp parallel for schedule(static) private(j) shared(achou) //Inicia a regiao paralela, com o schedule static, com "j" privado e "achou" compartilhado.
  for(j = 0; j < max; j++){
    if(achou == 1){//Se achou, as outras threads saem do loop
      exit(1); 
    }else{
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
        time (&t2); 
        dif = difftime (t2,t1);
        printf("\n%1.2f seconds\n", dif);
        achou = 1; // Torna a flag como verdadeira
        exit(1); // encerra essa thread
      }
    }
    
  }
}

int main(int argc, char **argv) {
  omp_set_num_threads(atoi(argv[2])); //define o número de threads.
  bruteForce(argv[1]);
  return 0;
}

