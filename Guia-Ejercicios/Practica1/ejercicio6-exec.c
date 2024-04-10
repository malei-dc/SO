#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void nosystem(const char *arg){
    pid_t hijo = fork();
    if (hijo == 0){
        execvp(arg); 
        printf("Error al ejecutar %s\n", arg);
        exit(EXIT_FAILURE);
    }
    else{
        wait(NULL);
        printf("quiero ejec %s \n", arg);
    }
    
}

int main(int argc, char const *argv[]){
    nosystem(argv[1]);
    exit(EXIT_SUCCESS); //cada uno finaliza su proceso
}
