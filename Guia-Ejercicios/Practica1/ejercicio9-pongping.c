#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


pid_t pid_hijo;
pid_t pid_padre;
int repetir = 0;

void handler_decirPong(int sig) {
    sleep(1);
    if (repetir < 3) {
        printf("pong %d - numero repe %d\n", pid_padre, repetir); 
        repetir++;
        kill(pid_hijo, SIGINT);
    }
    else {
        printf("Desea continuar? (si/no)\n");
        char respuesta[20];
        scanf("%s", respuesta);
        if (strcmp(respuesta, "si") == 0) {
            repetir = 0;
            kill(pid_padre, SIGINT);
        }
        else {
            kill(pid_hijo, SIGKILL);
            printf("Mate al hijo :) \n");
            exit(EXIT_SUCCESS);
        }
    }
}

void handler_decirPing(int sig) { 
    sleep(1);
	printf("ping %d - numero repe %d\n ", pid_hijo, repetir); 
	kill(pid_padre, SIGINT);
}

int main(int argc, char const *argv[]) {
    pid_padre = getpid();
	pid_t pid = fork();
    if (pid == 0) {
        //hijo
        pid_hijo = getpid();
        signal(SIGINT, handler_decirPing);
        while(1);
    }
    else {
        //padre
        pid_hijo = pid;
        signal(SIGINT, handler_decirPong);
        kill(pid_padre, SIGINT);
        while(1);
    }

	exit(EXIT_SUCCESS);
}