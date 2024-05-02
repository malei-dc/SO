#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <errno.h>

int n;
int rondas;
int numeroMaldito;
int numeroRandom;

int generate_random_number() {
	return (rand() % n);
}

void handler_hijo() {
    if (numeroMaldito == numeroRandom)
        exit(EXIT_SUCCESS);
}

void hijo() {
    signal(SIGTERM, handler_hijo);
    while (1) {
        numeroRandom = generate_random_number();
    // Tiene que estar generando numero aleatorio todo el tiempo, sino todos los hijos terminan con el mismo numero aleatorio
    }
}

int main(int argc, char* argv[]) {
	//N para la cantidad de procesos(menor a 10)
	//K la cantidad de rondas
	//J para el número maldito (J tiene que ser menor a N y mayor o igual 0).
    // Sacamos parametros de los argumentos
    n = atoi(argv[1]);
	rondas = atoi(argv[2]);
	numeroMaldito = atoi(argv[3]);

    // Vector de hijos
    pid_t vectorPidHijos[n];
    //creamos los hijos
    for (size_t i = 0; i < n; i++) {
        pid_t pidHijo = fork();
        if (pidHijo == 0) {
            hijo();
        } else
            vectorPidHijos[i] = pidHijo;
    }

    printf("YA CREAMOS TODOS LOS HIJOS \n QUE EMPIEZE EL JUEGO!\n\n");
    sleep(1); //espera que los hijos esten listo

    int muertos = 0;
    for (int i = 0; i < rondas; i++) {
        printf("EMPEZANDO LA RONDA %d\n", i + 1);

        //Mandamos SIGTERM a los hijos
        for (int i = 0; i < n; i++) {
            if (vectorPidHijos[i] != -1) {
                kill(vectorPidHijos[i], SIGTERM);
                sleep(1);
            }
        }

        //Esperan a que cada hijo termine
        for (int i = 0; i < n; i++) {
            int status;
            pid_t pid_terminado = waitpid(vectorPidHijos[i], &status, WNOHANG); //WNOHANG retorna si no hay un hijo que termine
            if (pid_terminado != 0){ // SI ES 0 CON WNOHANG, SIGUE VIVO (WAIT CON WNOHANG DEVUELVE EL PID DEL HIJO SI ES QUE SE MURIO)
                vectorPidHijos[i] = -1;
            }
        }

        //Imprimimos los hijos que sobrevivieron
        int muertos = 0;
        for (int i = 0; i < n; i++) {
            if (vectorPidHijos[i] != -1) {
                printf("EL HIJO %d con pid: %d SOBREVIVIO\n", i + 1, vectorPidHijos[i]);                    
            } else {
                muertos++;
            }
        }
        printf("cantidad muertos %d\n", muertos);
        //Si todos los hijos estan muertos, no hay necesidad de continuar
        if (muertos == n) {
            printf("TODOS MIS HIJOS ESTAN MUERTOS - NO HAY GANADORES\n\n");
            break; 
        }
        printf("TERMINANDO LA RONDA %d\n\n", i + 1);
    }

    printf("\n LOS GANADORES SON: \n");
    for (int i = 0; i < n; i++) {
        if (vectorPidHijos[i] != -1) {
            printf("EL HIJO %d con pid: %d \n", i + 1, vectorPidHijos[i]);                    
        } 
    }
    printf("PARTIDA TERMINADA, GRACIAS POR JUGAR\n");

    // Luego de terminar la partida, matamos a todos los hijos
    for (int i = 0; i < n; i++) {
        if (vectorPidHijos[i] != -1) {
            printf("PUM PUM PUM MATO HIJO %d con pid: %d\n", i + 1, vectorPidHijos[i]);
            kill(vectorPidHijos[i], SIGKILL);
        }
    }
    exit(EXIT_SUCCESS);
    

}