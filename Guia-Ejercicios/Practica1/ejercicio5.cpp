#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

void Subrutina_proceso_bart(){
    printf("soy bart\n");
}

void Subrutina_proceso_lisa(){
    printf("soy lisa\n");
}

void Subrutina_proceso_magie(){
    printf("soy magieeee\n");
}

void Subrutina_proceso_homero(){
	

    pid_t bart_pid = fork();
    if(bart_pid == 0){
        Subrutina_proceso_bart();
    }else{
        
        pid_t lisa_pid = fork();
        if(lisa_pid == 0){
            Subrutina_proceso_lisa();
        }else{
            pid_t magie_pid = fork();
            if(magie_pid == 0){
                Subrutina_proceso_magie();
            }else{
                wait(NULL);
                wait(NULL);
                wait(NULL);
                printf("Soy Homero\n");
            }
        }
    }
    
}

void Subrutina_proceso_Abraham(){
	printf("Soy Abraham\n");
}


int main(int argc, char const *argv[]){
  pid_t pid = fork();
  //si no hay error, pid vale 0 para el hijo
  //y el valor del process id del hijo para el padre
  wait(NULL);
  if (pid == -1) exit(EXIT_FAILURE);  
  //si es -1, hubo un error
  else if (pid == 0) {
     Subrutina_proceso_homero();
  }
  else {
     Subrutina_proceso_Abraham();
  }
  exit(EXIT_SUCCESS); //cada uno finaliza su proceso
}
