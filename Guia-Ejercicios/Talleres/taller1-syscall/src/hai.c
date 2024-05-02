#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int contador = 0;

void handlerPadre(int sig) {
	wait(NULL);
}

void handlerHijo(int sig) {
	printf("ya va! \n");
	contador++;
}

int main(int argc, char* argv[]) {
  	pid_t pidHijo = fork();
	if (pidHijo == 0) {
		//hijo
		signal(SIGURG, handlerHijo);
		while(1){
			if (contador == 5) break;
		}
		kill(getppid(), SIGINT);
		execvp(argv[1], argv+1);
		exit(EXIT_SUCCESS);
	}
	else {
		//padre
		signal(SIGINT, handlerPadre);
		sleep(1);
		int repe = 5;
		while (repe) {
			printf("sup!\n");
			kill(pidHijo, SIGURG);
			sleep(1);
			repe --;
		}
		
	}
	exit(EXIT_SUCCESS);
}
