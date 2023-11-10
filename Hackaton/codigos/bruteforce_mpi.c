/*
@(#)File:           bruteforce_mpi.c
@(#)Last changed:   Date: 11/24/2022 21:40:00
@(#)Purpose:        Brute Force Password Breaker with MultiProcessing
@(#)Modified:       Fernando Schettini, Gabriel Mascarenhas, Jadson Nobre
@(#)Usage:
 (*) Hotocompile:   mpicc bruteforce_mpi.c -o bruteforce_mpi -std=c99 -O3
 (*) Hotoexecute:   mpirun -np <number_of_processes> ./bruteforce_mpi <password>
                    mpirun -np 4 ./bruteforce_mpi senhatez 
@(#)Comment:
 (*)   Disponible in: http://forum.guiadohacker.com.br/showthread.php?t=40571
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

//97 to 122 use only lowercase letters
//65 to 90 use only capital letters
//48 to 57 use only numbers
#define START_CHAR 48
#define END_CHAR 122
#define MAXIMUM_PASSWORD 20

void bruteForce(char *pass, int np, int id);
long long my_pow(long long x, int y);

void bruteForce(char *pass, int np, int num_processo) {
  time_t t1, t2;
  double dif;
  int size;
  size = strlen(pass);
  t1 = time (&t1);
  int pass_b26[size];
  long long int j;
  long long int pass_decimal = 0;
  int base = END_CHAR-START_CHAR+2;
  
  for(int i = 0; i < size; i++){
    pass_b26[i] = (int)pass[i]-START_CHAR+1;
  }
  for(int i = size - 1; i > -1; i--){
    pass_decimal += (long long int) pass_b26[i]*my_pow(base,i);
  }

  long long int max = my_pow(base,size);
  char s[size];
  for(j = num_processo*max/np; j != (num_processo+1)*max/np; j++){ // divide o trabalho entre os processos, de acordo com o seu identificador, e o numero de processos totais, e o maximo de tentativas.
    if(j == pass_decimal){ //achou o processo
      int index = 0;
      while(j > 0){
        s[index++] = START_CHAR + j%base-1;
        j /= base;
      }
      s[index] = '\0';
      printf("Password encontrado pelo processo %d: %s\n", num_processo, s);
      t2 = time (&t2);
      dif = difftime(t2,t1);
      printf("Tempo de execução: %1.2f\n", dif);
      MPI_Abort( MPI_COMM_WORLD , 1); // encerra todos os processos 
    }
  }
}

long long my_pow(long long x, int y)
{
  long long res = 1;
  if (y==0)
    return res;
  else
    return x*my_pow(x,y-1);
}

int main(int argc, char **argv) {
  int np, num_processo;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &num_processo);
  bruteForce(argv[1], np, num_processo);
}

