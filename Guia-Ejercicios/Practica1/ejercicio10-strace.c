#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(int argc, char const *argv[]) {
	pid_t pid_juli = fork();
    if (pid_juli == 0) {
        //Julieta
        write(1, "Soy Julieta\n", 12);
        sleep(1);
        pid_t pid_juli = fork();
        if (pid_juli == 0) {
            //Jennifer
            write(1, "Soy Jennifer\n\0", 14);
            sleep(1);
        }
    }
    else {
        //Juan
         write(1, "Soy Juan\n\0", 10);
         sleep(1);
         wait(NULL);
         pid_t pid_jorge = fork();
         if (pid_jorge == 0) {
            //jorge
            write(1, "Soy Jorge\n", 10);
            sleep(1);
         }
    }
	exit(EXIT_SUCCESS);
}