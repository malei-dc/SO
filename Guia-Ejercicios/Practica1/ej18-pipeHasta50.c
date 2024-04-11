#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

enum {READ, WRITE};

int valor = 0;
int max = 50;


//necesito 2 pipes y 3 procesos (padre + 2 hijos)
//Padre -> hijo 1 -> hijo 2 -> Padre
//      0         1         2  <- pipes

void primerHijo(int pipe[][2]) {
    //cerrar extremos que no usamos
    close(pipe[2][READ]);
    close(pipe[2][WRITE]);
    close(pipe[0][WRITE]);
    close(pipe[1][READ]);
    //conectamos los que corresponde
    //dup2(pipe[0][READ], STDIN_FILENO); //entrada
    //dup2(pipe[1][WRITE], STDOUT_FILENO);//salida

    while (1) {
        read(pipe[0][READ], &valor, sizeof(valor));
        
        valor++;
        printf("Hijo_1 envía a Hijo_2 el valor %d\n", valor);
        write(pipe[1][WRITE], &valor, sizeof(valor));
        if (valor == max -1) break;
    }
    close(pipe[0][READ]);
    close(pipe[1][WRITE]);
    exit(EXIT_SUCCESS);
}

void segundoHijo(int pipe[][2]) {
    //cerrar extremos que no usamos
    close(pipe[0][READ]);
    close(pipe[0][WRITE]);
    close(pipe[1][WRITE]);
    close(pipe[2][READ]);
    //conectamos los que corresponde
    //dup2(pipe[1][READ], STDIN_FILENO); //entrada
    //dup2(pipe[2][WRITE], STDOUT_FILENO);//salida

    while (1) {
        read(pipe[1][READ], &valor, sizeof(valor));
    
        valor++;
        printf("Hijo_2 envía a Padre el valor %d\n", valor);
        write(pipe[2][WRITE], &valor, sizeof(valor));
        //Por algebra sé que el hijo 2 llega a 50
        if (valor == max) break;
    }
    close(pipe[1][READ]);
    close(pipe[2][WRITE]);
    exit(EXIT_SUCCESS);
}

void padre(int pipe[][2]) {
    //cerrar extremos que no usamos
    close(pipe[1][READ]);
    close(pipe[1][WRITE]);
    close(pipe[2][WRITE]);
    close(pipe[0][READ]);
    while(1) {
        printf("Padre envía a Hijo_1 el valor %d\n", valor);
        write(pipe[0][WRITE], &valor, sizeof(valor));

        read(pipe[2][READ], &valor, sizeof(valor));
        if (valor == 50) break;
        
        valor ++;

    }
    close(pipe[2][READ]);
    close(pipe[0][WRITE]);
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {
    int tunel[3][2]; //define pipe
    pipe(tunel[0]); pipe(tunel[1]); pipe(tunel[2]); //inicializo pipes

    pid_t hijo1, hijo2;
    hijo1 = fork();
    if (hijo1 == 0) {
        primerHijo(tunel);
    }
    else {
        hijo2 = fork();
        if (hijo2 == 0) {
            segundoHijo(tunel);
        }
        else {
            padre(tunel);
        }
    }
//En realidad como estoy usando read y write, si saco todos los close funciona igual ya que estamos mandando datos directo
}