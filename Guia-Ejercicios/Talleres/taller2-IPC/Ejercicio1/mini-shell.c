#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

enum { READ, WRITE }; //read: sale, write: entra

void ejecutar(char *cmd, char**p) {
    execvp(cmd, p);
}

void cierro_conectoPipes(int pipe[][2], int nroHijo, int count) {
	//cierro todos los pipes
	for (size_t i = 1; i < count - 1; i++) { //desde 1 al n-1
		if (i != nroHijo && ((i-1) != nroHijo)) {
			close(pipe[i][WRITE]);
			close(pipe[i][READ]);
		}
	}
	//conecto
	for (size_t i = 0; i < count; i++) {
		if (nroHijo == 0 && nroHijo == i) {
			close(pipe[i][WRITE]);
			dup2(pipe[i][READ], STDIN_FILENO);
		}
		else if (nroHijo == count -1 && nroHijo == i) {
			close(pipe[i-1][WRITE]);
			dup2(pipe[i- 1][READ], STDOUT_FILENO);
		}
		else {
			close(pipe[i-1][WRITE]);
			close(pipe[i][READ]);

			dup2(pipe[i-1][READ], STDIN_FILENO);
			dup2(pipe[i][WRITE], STDOUT_FILENO);
		}
	}
}

static int run(char ***progs, size_t count)
{	
	int r, status;

	//Reservo memoria para el arreglo de pids
	//TODO: Guardar el PID de cada proceso hijo creado en children[i]
	pid_t *children = malloc(sizeof(*children) * count);

	//TODO: Pensar cuantos procesos necesito: count procesos
	//TODO: Pensar cuantos pipes necesito: count -1 pipes
	int ultimoPipes = count - 2;

	int pipes[count - 1][2];
	//inicializo los pipes
	for (size_t i = 0; i < count; i++){
		pipe(pipes[i]);
	}
	//creo los hijos
	for (size_t i = 0; i < count; i++){
		pid_t pid = fork();
		if (pid == 0){
			//hijo i
			cierro_conectoPipes(pipes, i, count);
			ejecutar(progs[i][0], progs[i][0]+1);
		}
		else {
			children[i] = pid;
		}
	}
	
	//TODO: Para cada proceso hijo:
			//1. Redireccionar los file descriptors adecuados al proceso
			//2. Ejecutar el programa correspondiente

	
	//cierro todos los pipes del padre
	for (size_t i = 0; i < count; i++) {
		close(pipes[i][WRITE]);
		close(pipes[i][READ]);
	}

	//Espero a los hijos y verifico el estado que terminaron
	for (int i = 0; i < count; i++) {
		waitpid(children[i], &status, 0);

		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}
	r = 0;
	free(children);

	return r;
}


int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("El programa recibe como parametro de entrada un string con la linea de comandos a ejecutar. \n"); 
		printf("Por ejemplo ./mini-shell 'ls -a | grep anillo'\n");
		return 0;
	}
	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}
