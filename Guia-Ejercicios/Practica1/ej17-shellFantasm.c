#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum {READ, WRITE};

void ejecutar_cmd(char* cmd, char* p) {
    execlp(cmd, cmd, p, NULL);
}

void hijo1(int pipe[2]) {
    //cerrar lectura
    close(pipe[READ]);
    //conectar escritura a STDOUT
    dup2(pipe[WRITE], STDOUT_FILENO);
    ejecutar_cmd("ls", "-al");
}

void hijo2(int pipe[2]) {
    //cerrar escritura
    close(pipe[WRITE]);
    //conectar lectura con STDIN
    dup2(pipe[READ], STDIN_FILENO);
    ejecutar_cmd("wc",  "-l");
}

void main () {
    int tunel[2]; //define pipe
    pipe(tunel); //incializo pipe

    //creo hijos
    if (fork() == 0) {
        hijo1(tunel);
    }

    if (fork() == 0) {
        hijo2(tunel);
    }
    close(tunel[READ]);
    close(tunel[WRITE]);
}