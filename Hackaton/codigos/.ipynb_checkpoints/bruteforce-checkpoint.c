/*
@(#)File:           bruteforce.c
@(#)Last changed:   Date: 11/24/2022 21:40:00
@(#)Purpose:        Brute Force Password Breaker  
@(#)Modified:       Fernando Schettini, Gabriel Mascarenhas, Jadson Nobre
@(#)Usage:
 (*) Hotocompile:   gcc bruteforce.c -o bruteforce
 (*) Hotoexecute:   ./bruteforce <password>
                    ./bruteforce senhatez
@(#)Comment:
 (*)   Disponible in: http://forum.guiadohacker.com.br/showthread.php?t=40571
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//97 to 122 use only lowercase letters
//65 to 90 use only capital letters
//48 to 57 use only numbers
#define START_CHAR 48
#define END_CHAR 122
#define MAXIMUM_PASSWORD 20

void bruteForce(char *pass);
long long my_pow(long long x, int y);

int main(int argc, char **argv) {
  time_t t1, t2;
  double dif;
  time (&t1); // Captura tempo no começo do processo de brute force
  bruteForce(argv[1]);
  time (&t2); // Captura tempo no começo do processo de brute force

  dif = difftime (t2,t1); //Calcula tempo de execução baseado em t1 e t2
  printf("\n%1.2f seconds\n", dif);
  return 0;
}

void bruteForce(char *pass) {
  int size = strlen(pass);
  int pass_b26[size];
  long long int j;
  long long int pass_decimal = 0;
  int base = END_CHAR-START_CHAR+2;
  time_t t1, t2;
  double dif;

  //printf("base: %d\n", base);
  printf("Estamos tentando quebrar: %s\n", pass);

  time (&t1);
  //"Convertendo" a senha para base 26, pegando seu correspondente de 1 a 26 baseado na ordem do alfabeto.
  for(int i = 0; i < size; i++){
    pass_b26[i] = (int)pass[i]-START_CHAR+1; //Calculo para transformação
    //printf(" %d\n", pass_b26[i]);
  } 

  time (&t2);
  dif = difftime (t2,t1);
  printf("\nEste é o tempo do primeiro loop: %1.2f segundos\n", dif);

  time (&t1);
  //Convertendo a senha para um numero em base 10
  for(int i = size - 1; i > -1; i--){
    pass_decimal += (long long int) pass_b26[i]*my_pow(base,i);
    //printf("pass decimal: %lld\n", pass_decimal);
  }
  time (&t2);
  dif = difftime (t2,t1);
  printf("\nEste é o tempo do segundo loop: %1.2f segundos\n", dif);

  long long int max = my_pow(base,size); //Em tese, o tamanho maximo do numero do numero resultante da conversão da senha para base 26
  char s[size];

  time (&t1);
  //Laco que compara cada numero com a senha na base 10, caso encontre a senha é transformada novamente para base 26 e exibida na tela, encerrando o programa. 
  for(j = 0; j < max; j++){
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
      break;
    }
  }
  time (&t2);
  dif = difftime (t2,t1);
  printf("\nEste é o tempo do terceiro loop: %1.2f segundos\n", dif);
}

//Funcao recursiva para calculo de potencia
long long my_pow(long long x, int y)
{
  long long res = 1;
  if (y==0)
    return res;
  else
    return x*my_pow(x,y-1);
}