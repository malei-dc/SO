# Práctica 1: Procesos y API del SO

## 1) Cambio de contexto:
¿Cuáles son los pasos que deben llevarse a cabo para realizar un cambio de contexto?

        Guardar los registros.
        Guardar el IP.
        Si se trata de un programa nuevo, cargarlo en memoria.
        Cargar los registros del nuevo.
        Poner el valor del IP del nuevo.
        Otras cosas.

## 2) PCB (Process Control Block)
(a) Implementar la rutina Ke_context_switch(PCB* pcb_0, PCB* pcb_1). pcb_0 es el puntero al PCB del proceso a ser desalojado y pcb_1 al PCB del proceso a ser ejecutado a continuación.

	Ke_context_switch(PCB* pcb_0, PCB* pcb_1){
        pcb_0 -> pc = get_current_pc()
        pcb_0 -> registros = save_registers()
        set_current_process(pcb_1->pid)
        set_current_pc(pcb_1->pc)
        restore_registers(pcb_1->registers)
        ke_reset_current_user_time()
        ret()
    }
(b) Identificar en el programa escrito en el punto anterior cuáles son los pasos del ejercicio 1.

	Lineas 15 y 16 guardo estado actual. Lineas 17, 18 y 19 nuevo proceso.
 
## 3) System call vs llamada a función
Describir la diferencia entre un system call y una llamada a función de biblioteca.

    La syscall se llama a kernel, se necesita un cambio de privilegio, cambio de contexto, y consume tiempo.
    La llamada a funcion funcionan a nivel usuario, y algunas usan syscalls por dentro.

## 4) Transicion de estados
(a) Dibujar las puntas de flechas que correspondan. También puede agregar las transiciones que crea necesarias entre los estados disconexos y el resto.

(b) Explicar qué causa cada transición y qué componentes (scheduler, proceso, etc.) estarían involucrados.

   ![](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/foto4.png)

## 5) Fork
(a) Utilizando únicamente la llamada al sistema fork(), escribir un programa tal que construya un árbol de procesos que represente la siguiente genealogía: Abraham es padre de Homero, Homero es padre de Bart, Homero es padre de Lisa, Homero es padre de Maggie. Cada proceso debe imprimir por pantalla el nombre de la persona que representa.

[Código ej5-familia](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/ej05-familia.c)
    
(b) Modificar el programa anterior para que cumpla con las siguientes condiciones: 1) Homero termine sólo después que terminen Bart, Lisa y Maggie, y 2) Abraham termine sólo después que termine Homero.

[Código ej5-familia.c](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/ej05-familia.c) (Le puse 3 Wait a homero)

## 6) Exec
El ejercicio anterior es extendido con la llamada al sistema void exec(const char *****arg). Esta llamada al sistema reemplaza el programa actual por el código localizado en el string (char ***** arg). Implementar una llamada al sistema que tenga el mismo comportamiento que la llamada void system(const char ***** arg), usando las llamadas al sistema ofrecidas por el sistema operativo. Nota: Revisar man system, como ayuda.

[Código ej6-exec.c](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/ej06-exec.c)

## 7) Interfaz POSIX 
Programar en C el ejercicio 5b y 6.

[Código ej5-familia](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/ej05-familia.c)
[Código ej6-exec.c](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/ej06-exec.c)

## 8) Analizando codigo
Veamos el siguiente fragmento de código de un fork

	int main(int argc, char const *argv[]){
		int dato = 0;
		pid_t pid = fork();
		//si no hay error, pid vale 0 para el hijo
		//y el valor del process id del hijo para el padre
		if (pid == -1) exit(EXIT_FAILURE);
			//si es -1, hubo un error
		else if (pid == 0) {
			for (int i=0; i< 3; i++) {
				dato++;
				printf("Dato hijo: %d\n", dato);
			}
		}
		else {
			for (int i=0; i< 3; i++) {
				printf("Dato padre: %d\n", dato);
			}
		}
		exit(EXIT_SUCCESS); //cada uno finaliza su proceso
	}

¿Son iguales los resultados mostrados de la variable dato para el padre y para el hijo? ¿Qué está sucediendo?

    No, el padre devuelve 0 tres veces, y el hijo devuelve 1 2 3. 
    Esto sucede porque al "forkear" se crea un proceso hijo con la copia del codigo del padre (a menos que se haga un exec que no es el caso) y solo se suma al valor dato en el hijo pero no en el padre. 

## 9) Señales POSIX
Dado un programa de dos procesos, padre e hijo, se quiere tener el siguiente comportamiento:
Uno de los dos procesos debe escribir en pantalla ping y su número de PID. Automáticamente el otro proceso debe escribir pong con su número de PID. Se quiere repetir este comportamiento 3 veces. Luego de esto, se desea preguntar al usuario si quiere finalizar la ejecución o no. En caso que conteste que si, el padre debe terminar con la ejecución de su hijo y finalizar. En caso que se conteste que no, se vuelve a repetir el proceso antes dicho.

[Código ej9-pongping.c](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/ej09-pongping.c)

## 10) Strace
Dado el siguiente fragmento de strace. Escribir el código correspondiente.

    clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLDstrace: Proce
    [pid 10551] write(1, "Soy Juan\n\0", 10) = 10
    [pid 10552] write(1, "Soy Julieta\n", 12 <unfinished ...>
    [pid 10551] clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=1, tv_nsec=0}, <unfinished ...>
    [pid 10552] clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=1, tv_nsec=0}, <unfinished ...>
    [pid 10551] wait4(-1, <unfinished ...>
    [pid 10552] clone(child_stack=NULL,
    [pid 10557] write(1, "Soy Jennifer\n\0", 14 <unfinished ...>
    [pid 10552] exit_group(0)
    [pid 10557] clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=1, tv_nsec=0}, <unfinished ...>
    [pid 10551] <... wait4 resumed>[{WIFEXITED(s) && WEXITSTATUS(s) == 0}], 0, NULL) = 10552
    [pid 10551] --- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=10552, si_uid=1000,
    [pid 10551] clone(child_stack=NULL, 
    [pid 10551] exit_group(0)
    [pid 10558] write(1, "Soy Jorge\n", 10) = 10
    [pid 10558] clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=1, tv_nsec=0}, <unfinished ...>
    [pid 10557] <... clock_nanosleep resumed>0x7ffe6cd07ca0) = 0
    [pid 10558] <... clock_nanosleep resumed>0x7ffe6cd07ca0) = 0

Comportamiento: Juan tiene a Julieta y Jorge como hijos. Julieta tiene a Jeniffer como hija

[Código ej10-strace.c](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/ej10-strace.c)

	gcc -o strace ej10-strace.c 
	strace -q -f ./strace

Ejecutar strace del código que realizó y contrastarlos. ¿Qué comando exacto utilizó para strace?
 
    strace obtenido:
    clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD 
    [pid  6050] write(1, "Soy Juan\n\0", 10 <unfinished ...>
    [pid  6051] set_robust_list(0x70d3bf9c5a20, Soy Juan) = 0
    [pid  6050] <... write resumed>)        = 10
    [pid  6050] clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=1, tv_nsec=0},  <unfinished ...>
    [pid  6051] write(1, "Soy Julieta\n", Soy Julieta) = 12
    [pid  6051] clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=1, tv_nsec=0},  <unfinished ...>
    [pid  6050] wait4(-1,  <unfinished ...>
    [pid  6051] clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD,
    [pid  6054] set_robust_list(0x70d3bf9c5a20, 24) = 0
    [pid  6054] write(1, "Soy Jennifer\n\0", 14 <unfinished ...>
    [pid  6054] clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=1, tv_nsec=0},  <unfinished ...>
    [pid  6051] +++ exited with 0 +++
    [pid  6050] <... wait4 resumed>NULL, 0, NULL) = 6051
    [pid  6050] --- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=6051, si_uid=1000
    [pid  6050] clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD,
    [pid  6055] set_robust_list(0x70d3bf9c5a20, 24) = 0
    [pid  6050] exit_group(0)               = ?
    [pid  6055] write(1, "Soy Jorge\n", Soy Jorge) = 10
    [pid  6055] clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=1, tv_nsec=0},  <unfinished ...>
    [pid  6054] <... clock_nanosleep resumed>0x7ffce34569f0) = 0
    [pid  6054] exit_group(0)               = ?
    
## 11) bsend y breceive
Un nuevo sistema operativo ofrece:
- void bsend(pid dst, int msg): Envía el valor msg al proceso dst.
- int breceive(pid src): Recibe un mensaje del proceso src.
- pid get_current_pid(): que devuelve el process id del proceso que invoca dicha llamada.

(a) Escribir un programa que cree un segundo proceso, para luego efectuar la siguiente secuencia de mensajes entre ambos:

1. Padre envía a Hijo el valor 0,
2. Hijo envía a Padre el valor 1,
3. Padre envía a Hijo el valor 2,
4. Hijo envía a Padre el valor 3,
5. etc.
----

	int value = 0;
	void hijo() {
		while(1) {
			value = breceive(pidPadre);
			value++;
			bsend(pidPadre, value); }
	}

    void padre() {
        while(1) {
            bsend(pidHijo, value);
            value = breceive(pidHijo);
            value++;}
    }

    int main(int argc, char const *argv[]) {
        pidPadre = getpid();
        pidHijo = fork();
        if (pidHijo == 0) {
            pidHijo = getpid();
            hijo(); }
        else
            padre();
        return 0;
    }

(b) Modificar el programa anterior para que cumpla con las siguientes condiciones: 1) Padre cree dos procesos hijos en lugar de uno, y 2) se respete esta nueva secuencia de mensajes entre los tres procesos.

1. Padre envía a Hijo_1 el valor 0,
2. Hijo_1 envía a Hijo_2 el valor 1,
3. Hijo_2 envía a Padre el valor 2,
4. Padre envía a Hijo_1 el valor 3,
 ...hasta llegar al valor 50.
----

    int value = 0; int max = 50; //el hijo 2 llega a 50

    void hijo() {
        pidhijo2 = breceive(pidPadre);
        while (1) {
            value = breceive(pidPadre);
            value ++;
            bsend(pidhijo2, value); 
            if (value == 49) break;
        }
        exit(EXIT_SUCCESS);
    }

    void hijo2() {
        while (1) {
            value = breceive(pidHijo1);
            value ++;
            bsend(pidPadre, value); 
            if (value == 50) break;
        }
        exit(EXIT_SUCCESS);
    }

    void padre() {
        bsend(pidHijo, pidHijo2); // hijo no conoce hijo2
        while(1) {
            bsend(pidHijo, value);
            value = breceive(pidHijo2);
            if (value == 50) break;
            value ++;
        }
        exit(EXIT_SUCCESS);
    }

    int main(int argc, char const *argv[]) {
        pidPadre = getpid();
        pidHijo = fork();
        if (pidHijo == 0) {
            pidHijo = getpid();
            hijo(); }
        else {
            pidHijo2 = fork();
            if (pidHijo2 == 0) {
                pidHijo2 = getpid();
                hijo2();
            } else
                padre();
        }
    }

## 12) Funciones bloqueantes
Dado dos procesos que ejecutan cómputo_muy_difícil_1() y cómputo_muy_difícil_2() respectivamente. Ambos procesos se conocen mutuamente a través de las variables pid_derecha y pid_izquierda.

	int result;
	void proceso_izquierda() {
	   result = 0;
	   while (true) {
 	     bsend(pid_derecha, result);
	     result = cómputo_muy_difícil_1();
	   }
	}
	
	void proceso_derecha() {
	   while(true) {
	      result = cómputo_muy_difícil_2();
	      int left_result = breceive(pid_izquierda);
	      printf("%s %s", left_result, result);
	   }
	}

(a) Sea la siguiente secuencia de uso de los procesadores para ejecutar los procedimientos costosos.

| Tiempo | Procesador 1 | Procesador 2 |
| :------------ |:---------------:| -----:|
| 1 | cómputo_muy_difícil_1 | cómputo_muy_difícil_2 |
| 2 | cómputo_muy_difícil_1 | cómputo_muy_difícil_2 |
| 3 | cómputo_muy_difícil_1 | cómputo_muy_difícil_2 |
|...|... |  ... |


¿Por qué esta secuencia no es realizable en el sistema operativo descripto?

	bsend y breceive son bloqueantes, no se ejecuta cómputo_muy_difícil_1 hasta que se haya computado el cómputo_muy_difícil_2 por primera vez

Escribir una secuencia que sí lo sea: 

| Tiempo | Procesador 1 | Procesador 2 |
| :------------ |:---------------:| --------:|
| 1 |       -        |  cómputo_muy_difícil_2 |
| 2 |      cómputo_muy_difícil_1  | cómputo_muy_difícil_2|
| 3 |     cómputo_muy_difícil_1  | cómputo_muy_difícil_2|
|    |                 ...        |             ...|

(b) ¿Qué cambios podría hacer al sistema operativo de modo de lograr la secuencia descripta en el punto anterior?

        Hacerlos no bloqueantes.

## 13) Comunicación entre procesos
Mencionar y justificar qué tipo de sistema de comunicación (basado en memoria compartida o en pasaje de mensajes) sería mejor usar en cada uno de los siguientes escenarios:

(a) Los procesos cortarBordes y eliminarOjosRojos necesitan modificar un cierto archivo foto.jpg al mismo tiempo.

        Memoria compartida, no son necesarios comunicarse entre sí

(b) El proceso cortarBordes se ejecuta primero y luego de alguna forma le avisa al proceso eliminarOjosRojos para que realice su parte.

	Pasaje de mensajes,  ya que se realiza de a partes y uno le tiene que avisar cuando termina.

(c) El proceso cortarBordes se ejecuta en una casa de fotos. El proceso eliminarOjosRojos es mantenido en tan estricto secreto que la computadora que lo ejecuta se encuentra en la bóveda de un banco.

    Pasaje de mensajes, este caso tiene que mantener la confidencialidad del mensaje, pero no deja de ser pasaje de mensajes.

## 14) send y receive
Un sistema operativo provee:
- bool send(pid dst, int *msg): Envía al proceso dst el valor del puntero. Retorna false si la cola de mensajes estaba llena.
- bool receive(pid src, int *msg): Recibe del proceso src el valor del puntero. Retorna false si la cola de mensajes estaba vacía.

(a) Modificar el programa del ejercicio 12 para que utilice estas llamadas al sistema.

    int result;
    void proceso_izquierda() {
        result = 0;
        while (true) {
            bool msgSent = false;
            while (!msgSent) {
                msgSent = send(pid_derecha, &result);
            }
            result = cómputo_muy_difícil_1();
        }
    }
    void proceso_derecha() {
        while(true) {
            result = cómputo_muy_difícil_2();
            int left_result;
            bool msgReceived = false;
            while (!msgReceived) {
                msgReceived = receive(pid_izquierda, &left_result);
            }
            printf("%s %s", left_result, result);
        }
    }

(b) ¿Qué capacidad debe tener la cola de mensajes para garantizar el mismo comportamiento?

    La cola de msg debe tener la capacidad de almacenar exactamente un entero par que tenga el mismo resultado.

## 15) SO de Solo Pipes/memoria compartida/sockets
¿Qué sucedería si un sistema operativo implementara pipes como único sistema de comunicación interprocesos? 

    Ventajas: 1. Simplicidad
              2. Buena Comunicación unidireccional
              3. Recurso compartido limitados
    Desventajas: 1. Comunicación limitada (solo admiten en procesos padre-hijo)
                 2. Unidireccional
                 3. Limitacion en escalabilidad
                 4. Sincronizacion manual.

¿Qué ventajas tendría incorporar memoria compartida?

    Ventajas: 1. Comunicación eficiente
              2. Bidireccional
              3. Escalabilidad
              4. Sincronizacion

¿Y sockets?

    Ventajas: 1. Comunicación a través de la red
              2. Protocolo de comunicación flexibles
              3. Escalabilidad en redes
              4. Amplia adopción
              5. + ventajas para entornos distribuidos.

## 16) Comunicación bloqueantes/No bloqueantes
Pensar un escenario donde tenga sentido que dos procesos (o aplicaciones) tengan entre sí un canal de comunicaciones bloqueante y otro no bloqueante. Describir en pseudocódigo el comportamiento de esos procesos.

    Edición de texto en tiempo real. Dos procesos -> "usuario 1", "usuario 2" y un servidor
        Proceso "usuario 1": ("usuario 2" análogo)
            1. Conectar a un servidor compartido.
            2. Abrir doc para edición.
            3. Mientras se hace ediciones en su copia:
                a. Envía ediciones locales al servidor:
                    I. Usar un canal de comunicación no bloqueante para enviar cambios a tiempo real al servidor.
                    II. Continuar trabajando en el doc local mientras se espera confirm del server.
            4. Recibir actualizaciones en tiempo real del doc desde el server:
                I. Usar un canal de comunicación bloqueante para recibir actualización en tiempo real.
                II. Actualizar los cambios realizados por otros usuarios.
                III. Mostrar los cambios de otros usuarios a "usuario 1".
            5. Repetir 3, 4 mientras se siga editando.
            6. Cerrar doc: mandar señal al server y cerrar comunicación.

## 17) Pipe
Escribir el código de un programa que se comporte de la misma manera que la ejecución del comando “ls -al | wc -l” en una shell. 
No está permitido utilizar la función system, y cada uno de los programas involucrados en la ejecución del comando deberá ejecutarse como un subproceso.

[Código ej17-shellFantasm.c](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/ej17-shellFantasm.c)

## 18) Pipe - padre/dos hijos
Implementar el inciso b del ejercicio 11 usando pipes en C. Determinar si el comportamiento del intercambio de mensajes obtenido es igual al especificado por las funciones bsend y breceive.

[Código ej18-pipeHasta50.c](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica1/ej18-pipeHasta50.c)

## 19) ...
Se desea hacer un programa que corra sobre una arquitectura con 8 núcleos... No se que pretende de mi este ej, paso :)

## 20) Juego numero - ej Taller1 con pipes

En el [taller1](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Talleres/taller1-syscall)

## 21) Juego BangBangBang - Taller1
