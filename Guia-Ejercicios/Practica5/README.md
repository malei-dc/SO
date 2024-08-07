# Práctica 5: Entrada/Salida

## 1) Concepto Driver
¿Cuáles de las siguientes opciones describen el concepto de driver? Seleccione las correctas y justifique.
    
(a) Es una pieza de software. 

>Correcto. Un driver es un programa de software que permite al sistema operativo y a otros programas interactuar con el hardware. Los drivers actúan como traductores entre el dispositivo hardware y el Kernel.

(b) Es una pieza de hardware.

>Incorrecto. Un driver es software, no hardware.

(c) Es parte del SO.

>Correcto. Los drivers son generalmente componentes del sistema operativo, ya que están integrados o cargados por el sistema operativo para gestionar la comunicación con el hardware. Sin embargo, no son parte del núcleo del sistema operativo en sí, pero sí son esenciales para la operación de muchos dispositivos.

(d) Dado que el usuario puede cambiarlo, es una aplicación de usuario.

>Incorrecto. Aunque los usuarios pueden instalar o actualizar drivers, estos no son considerados aplicaciones de usuario, ya que no son programas que los usuarios finales ejecuten directamente para realizar tareas. Los drivers funcionan en un nivel más bajo para facilitar la comunicación entre el hardware y el software del sistema.

(e) Es un gestor de interrupciones.

>Incorrecto. Aunque los drivers pueden manejar interrupciones generadas por el hardware, no son gestores de interrupciones per se. Los gestores de interrupciones son componentes específicos del sistema operativo que manejan la asignación y la respuesta a las interrupciones.

(f) Tiene conocimiento del dispositivo que controla pero no del SO en el que corre.

>Incorrecto. Los drivers deben tener conocimiento del sistema operativo para poder funcionar correctamente, ya que necesitan interactuar con él para realizar tareas como la gestión de memoria, la comunicación con el hardware, y la implementación de políticas de seguridad.

(g) Tiene conocimiento del SO en el que corre y del tipo de dispositivo que controla, pero no de las particularidades del modelo específico.

>Correcto. Un driver tiene que ser consciente del sistema operativo en el que se está ejecutando para poder interactuar correctamente con él y debe conocer cómo controlar el tipo de dispositivo para el que fue diseñado. Sin embargo, los drivers están usualmente diseñados para ser genéricos respecto a las particularidades del modelo específico de un dispositivo, aunque en algunos casos pueden tener optimizaciones específicas.

## 2) Cronómetro
Un cronómetro posee 2 registros de E/S:

- CHRONO_CURRENT_TIME que permite leer el tiempo medido,
- CHRONO_CTRL que permite ordenar al dispositivo que reinicie el contador.

El cronómetro reinicia su contador escribiendo la constante CHRONO_RESET en el registro de control.
Escribir un driver para manejar este cronómetro. Este driver debe devolver el tiempo actual cuando invoca la operación read(). Si el usuario invoca la operación write(), el cronómetro debe reiniciarse.

    int main (int argc, char *argv[]) {
        int crono = open("/dev/crono", "w");
        char *respuesta[2];
        int reinicio;
        while (1) {
            printf("Desea reiniciar el cronometro? SI/NO\n");
            scanf ("%s", &respuesta);
            if (respuesta == "SI")
                reinicio = write()
            else 
                reinicio = read()
				
            if (reinicio == 1)
                printf("Cronómetro reiniciado\n");
            else 
                printf("Tiempo actual: %s\n", reinicio);
        }
    }
	
	//Funciones del driver
    semaphore mutex;
    int driver_init() {
        mutex = sem(1);
        return DRI_OK;          //Un entero que represente que termina bien
    }

    int driver_read(int* udata) {
        mutex.wait();
        int time = IN(CHRONO_CURRENT_TIME);
        copy_to_user(udata, &time, sizeof(time));
        mutex.signal();
        return sizeof(time);
    }

    int driver_write(int* udata) {
        mutex.wait();
        OUT(CHRONO_CTRL, CHRONO_RESET);
        OUT(CHRONO_CURRENT_TIME, 0);
        mutex.signal();
        return DRI_OK;
    }

## 3) Boton
Una tecla posee un único registro de E/S : BTN_STATUS. Solo el bit menos significativo y el segundo bit menos significativo son de interés:

- BTN_STATUS0 : vale 0 si la tecla no fue pulsada, 1 si fue pulsada.
- BTN_STATUS1 : escribir 0 en este bit para limpiar la memoria de la tecla.

Escribir un driver para manejar este dispositivo de E/S. El driver debe retornar la constante BTN_PRESSED cuando se presiona la tecla. Usar busy waiting.

    int driver_init() {
        mutex = sem(1);
        return DRI_OK;          
    }

    int driver_write(int *udata) {
        mutex.wait();
        int statos;
        // Busy-waiting hasta que el bit menos significativo sea 1.
        while ((statos = IN(BTN_STATUS)) and 1 != 0)
        // Reseteamos el estado de la tecla.
        OUT(BTN_STATUS, pongoElBit1En0(status));
        mutex.signal();

        int result = BTN_PRESSED;
        copy_to_user(udata, &result, sizeof(result));
        
        return DRI_OK;
    }

## 4) Boton con interrupción
Reescribir el driver del ejercicio anterior para que utilice interrupciones en lugar de busy waiting.
Para ello, aprovechar que la tecla ha sido conectada a la línea de interrupción número 7.
Para indicar al dispositivo que debe efectuar una nueva interrupción al detectar una nueva pulsación de la tecla, debe guardar la constante BTN_INT en el registro de la tecla.

    int driver_init() {
        mutex = sem(1);
        presionado = sem(0);
        irq_register(7, handler)
        // Reseteamos el estado de la tecla para que mande la primer interrupción.
        OUT(BTN_STATUS, BTN_INT);
        return DRI_OK;          
    }

    void handler() {
        presionado.signal();
    }

    int driver_remove() {
        free_irq(7);
        return DRI_OK;
    }

    int driver_write(int *udata) {
        // Esperamos el mutex para tener uso exclusivo de la tecla.
        mutex.wait();
        // Esperamos la señal de que la tecla fue presionada.
        presionado.wait();
        OUT(BTN_STATUS, BTN_INT);
        mutex.signal();

        // Avisamos al usuario que se presionó la tecla.
        int result = BTN_PRESSED;
        copy_to_user(udata, &result, sizeof(result));
        
        return sizeof(result);
    }

## 5) Open - write
Indicar las acciones que debe tomar el administrador de E/S:
(a) cuando se efectúa un open().

>- Comprobar si el archivo o dispositivo solicitado existe y es accesible.
>- Inicializar cualquier estado necesario para la operación del archivo o dispositivo. Esto puede incluir buffers de E/S, bloques de control, handler de interrupciones, etc.
>- Verificar que el usuario tiene los permisos adecuados (lectura, escritura, ejecución) para acceder al archivo o dispositivo solicitado.
>- Actualizar las tablas de archivos abiertos y otras estructuras internas del sistema operativo para reflejar el nuevo archivo o dispositivo abierto.

(b) cuando se efectúa un write().

>- Comprobar que el descriptor de archivo proporcionado es válido y corresponde a un archivo o dispositivo abierto en modo de escritura.
>- Verificar que el usuario tiene permisos de escritura para el archivo o dispositivo.
>- Copiar los datos que se van a escribir desde el espacio de usuario (la memoria del proceso que llamó a write()) al espacio de memoria del kernel, donde el sistema operativo puede procesarlos de manera segura.
>- Realizar la escritura física de los datos en el archivo o dispositivo. Esto puede implicar la actualización de los bloques de datos en un sistema de archivos o la transmisión de datos a través de un dispositivo de red o un dispositivo de almacenamiento.
>- Actualizar la posición actual en el archivo o dispositivo para reflejar los datos escritos. Esto permite que las subsecuentes operaciones de E/S continúen desde la posición correcta.

## 6) IN - OUT
¿Cuál debería ser el nivel de acceso para las syscalls IN y OUT? ¿Por qué?

>Las syscalls IN y OUT se ejecutan en nivel kernel porque acceden de forma directa a los registros de los dispositivos de hardware. 
De esta forma un programa de usuario malicioso o mal implementado no puede acceder directamente a los dispositivos, necesita pasar por un driver, permitiendo así que el SO aplique los mecanismos de protección y seguridad. Por ejemplo, el programa de usuario necesita tener los permisos suficientes para hacerle open al driver.

## 7) Discos ópticos
Se desea implementar el driver de una controladora de una unidad de discos ópticos que requiere controlar manualmente el motor de la misma. 
Esta controladora posee 3 registros de lectura y 3 de escritura. 

Los registros de escritura son:

- DOR_IO: enciende (escribiendo 1) o apaga (escribiendo 0) el motor de la unidad.
- ARM: número de pista a seleccionar.
- SEEK_SECTOR: número de sector a seleccionar dentro de la pista.

Los registros de lectura son:

- DOR_STATUS: contiene el valor 0 si el motor está apagado (o en proceso de apagarse), 1 si está encendido. Un valor 1 en este registro no garantiza que la velocidad rotacional del motor sea la suficiente como para realizar exitosamente una operación en el disco.
- ARM_STATUS: contiene el valor 0 si el brazo se está moviendo, 1 si se ubica en la pista indicada en el registro ARM.
- DATA_READY: contiene el valor 1 cuando el dato ya fue enviado.

Además, se cuenta con las siguientes funciones auxiliares (ya implementadas):

- int *cantidad_sectores_por_pista*(): Devuelve la cantidad de sectores por cada pista del disco. El sector 0 es el primer sector de la pista.
- void *escribir_datos*(void *src): Escribe los datos apuntados por src en el último sector seleccionado.
- void *sleep*(int ms): Espera durante ms milisegundos.

Antes de escribir un sector, el driver debe asegurarse que el motor se encuentre encendido. Si no lo está, debe encenderlo, y para garantizar que la velocidad rotacional sea suficiente, esperar al menos 50 ms antes de realizar cualquier operación. A su vez, para conservar energía, una vez qwriteue finalice una operación en el disco, el motor debe ser apagado. El proceso de apagado demora como máximo 200 ms, tiempo antes del cual no es posible comenzar nuevas operaciones.

(a) Implementar la función write(int sector, void *data) del driver, que escriba los datos apuntados por data en el sector en formato LBA (es decir, que los sectores son numerados según un índice, empezando por cero) indicado por sector. Para esta primera implementación, no usar interrupciones.

        int driver_init() {
            mutex = sem(1);
            return DRI_OK;
        }

        int write(int sector, void *data) {
            mutex.wait()
            int uSector;
            copy_from_user(&uSector, sector, sizeof(int));

            //calculo pista y sector que se van a escribir en los registros de escritura. El enunciado no especifica el cálculo.
            int pista = divisionParteEntera(uSector, cantidad_sectores_por_pista())
            uSector = uSector % cantidad_sectores_por_pista();

            //me aseguro que el motor este encendido
            if (!IN(DOR_STATUS))
                OUT(DOR_IO, 1)              //si motor estaba apagado encender
            sleep(55);                      //en cualquier caso esperar 55ms para asegurar velocidad

            //insertamos datos
            OUT(ARM, pista);
            OUT(SEEK_SECTOR, uSector);

            //esperamos a que encuentre pista
            while(!IN(ARM_STATUS)) {}

            //escribimos data
            int datos
            copy_from_user(&datos, data, sizeof(int))
            escribir_datos(*datos);

            //esperamos a que termine la escritura
            while(!IN(DATA_READY)) {}

            OUT(DOR_IO, 0);
            sleep(200);
            mutex.signal()
            return DRI_OK;
        }

(b) Modificar la función del inciso anterior utilizando interrupciones. La controladora del disco realiza una interrupción en el IRQ 6 cada vez que los registros ARM_STATUS o DATA_READY toman el valor 1. Además, el sistema ofrece un timer que realiza una interrupción en el IRQ 7 una vez cada 50ms. Para este inciso, no se puede utilizar la función sleep.

        int driver_init() {
            atomic<int> contador = 0;
            ready_data = sem(0);
            ready_arm = sem(0);
            ready_motor = sem(0);
            ready_vel_motor = sem(0)
            mutex = sem(1);
            irq_register(6, handler_regs);
            irq_register(7, handler_vel);
            return DRI_OK;
        }

        int driver_remove() {
            irq_free(6);
            irq_free(7);
            return DRI_OK;
        }

        void handler_regs() {
            //Creo que nunca se ponen 1 a la vez
            if (IN(ARM_STATUS) == 1) 
                sem_signal(&ready_arm);
            if (IN(DATA_READY) == 1) 
                sem_signal(&ready_data);
        }

        void handler_vel() {
        //--DUDA: como hacemos para controlar la vel si se llama cada 50ms?--
        //solucion: mandar signal cada 2 interrupciones, contar con contador atomico
            contador.fetchAdd(1);
            if (contador % 2 == 1)
                ready_motor.signal();
            else 
                ready_vel_motor.signal();
        }

        int write(int sector, void *data) {
            mutex.wait()
            int uSector;
            copy_from_user(&uSector, sector, sizeof(int));

                //calculo pista y sector que se van a escribir en los registros de escritura. El enunciado no especifica el calculo.
            int pista = divisionParteEntera(uSector, cantidad_sectores_por_pista())
            uSector = uSector % cantidad_sectores_por_pista();

            ready_motor.wait()
                //enciendo motor
            if (!IN(DOR_STATUS))
                OUT(DOR_IO, 1)              //si motor estaba apagado encender

                //nos aseguramos que esté en la velocidad correcta
            ready_vel_motor.wait()

                //insertamos datos
            OUT(ARM, pista);
            OUT(SEEK_SECTOR, uSector);

                //esperamos a que encuentre pista
            ready_arm.wait();

                //escribimos data
            int datos
            copy_from_user(&datos, data, sizeof(int))
            escribir_datos(*datos);

                //esperamos a que termine la escritura
            ready_data.wait()

            OUT(DOR_IO, 0);
    
            mutex.signal()
            return DRI_OK;
        }

## 8) Impresora rara
Se desea escribir un driver para la famosa impresora rara. El manual del controlador nos dice que para comenzar una impresión, se debe:

- Ingresar en el reg de 32 bits la dirección de memoria dónde empieza el buffer que contiene el string a imprimir.
- Ingresar en el reg de 32 bits LOC_TEXT_SIZE la cantidad de caracteres que se deben leer del buffer.
- Colocar la constante START en el registro LOC_CTRL.

Registros de lectura: 

- LOC_CTRL:
	- No ha tinta -> LOW_INK
	- Imprimiendo -> PRINTING
	- Terminado   -> FINISHED

- LOC_STATUS: 
	- No hay tinta -> READY
	- Imprimiendo  -> BUSY
	- Terminado    -> READY

- Interrupciones: 
	- HP_LOW_INK_INT -> bajo tinta
	- HP_FINISHED_INT -> termina impresion

---

Por la mala calidad del hardware, éstas impresoras suelen detectar erróneamente bajo nivel de tinta.
Sin embargo, el fabricante nos asegura “alcanza con probar hasta 5 veces para saber con certeza si hay o no nivel bajo de tinta”.
Implementar las funciones int driver_init(), int driver_remove() y int driver_write(). 

    int driver_init() {
        mutex = sem(1);
        semFinish = sem(0);
        semHayTinta = sem(0);
        irq_register(HP_FINISHED_INT, handler_finish);
        irq_register(HP_LOW_INK_INT, handler_ink);
        return DRI_OK;
    }

    int driver_remove() {
        irq_free(HP_FINISHED_INT);
        irq_free(HP_LOW_INK_INT);
        contador = 0;
        return DRI_OK;
    }
	
    void handler_ink () {}
    
    void handler_finish() {
        semFinish.signal();
    }

    int driver_write(void* udata, int size) {
        int status = size;

        mutex.wait();
            //traigo el la data de usuario
        void *kdata = kmalloc(size, GFP_KERNEL)
        copy_from_user(kdata, udata, size);

            //ingreso los valores a los registros correspondientes
        OUT(LOC_TEXT_POINTER, kdata);
        OUT(LOC_TEXT_SIZE, size);

            //vemos si hay tinta hasta 5 veces, hacemos caso solo al ultimo estado
        int intentos = 5;
        while (intentos) {
            OUT(LOC_CTRL, START);
            while (IN(LOC_CTRL) == START) {}  //busy waiting hasta que cambie de estado, puede ser LOW_INK, PRINTING, FINISHED
            if (IN(LOC_CTRL) == PRINTING) 
                break;                        //tiene tinta y empezo a imprimir
            intentos --;
        }

        if (intentos == 0) {
            status = DRI_ERROR;
        }
        else {
            semFinish.wait()    //esperamos a que llegue la interrupcion de que termino
        }
        kfree(kdata);
        mutex.signal();

        return status;
    }

>Usamos una mezcla de polling e interrupción. Hacemos busy waiting con los estados de los registros para arrancar por la posible deteccion erronea de tinta y usamos interrupcion para detectar cuando termina de imprimir ya que no tiene problemas eso. 

## 9) Sistema de seguridad
Se requiere diseñar un sistema de seguridad compuesto por una cámara, un sensor de movimiento, y un software de control. El requisito principal es que cuando el sensor detecta movimiento, el sistema responda con el encendido de la cámara por una cantidad de tiempo T, si detecta movimiento antes de que termine el tiempo T, la política a seguir es la de esperar un tiempo T desde esta última detección.

(a) Proponga un diseño para este sistema de seguridad, donde debe indicar cuántos y qué tipo de registros tendría cada dispositivo, e indicando también para qué se utilizarían. También debe indicar y justificar el tipo de interacción: interrupciones, polling, DMA, etc. con cada dispositivo. Puede usar uno o más tipos de interacción diferentes para cada dispositivo.

>Tenemos dos dispositivos: camara y sensor.
>Cámara: 
>- POWER: registro que contiene el tiempo que tiene que estar prendido. La cámara se activa solo cuando el sensor detecta movimiento, tiene que hacer un write() para escribir en el registro POWER.
>
>Sensor:
>- Interrupcion 6 que se activa cuando detecta movimiento. Tiene que hacer un read() ya que cambia el estado del dispositivo. La funcion read() de sensor será una función bloqueante cuando no detecta movimiento.
>
>Software de control:
>Será el software que corre a nivel usuario donde tiene que abrir los dipositivos involucrados y llamar las funciones correspondientes para el correcto funcionamiento del sistema de seguridad.

(b) Una vez que tenga el diseño, debe escribir los dos drivers correspondientes (las operaciones que considere necesarias: open(), write(), read(), etc, para poder cumplir el objetivo planteado. Tenga en cuenta que el software de control correrá a nivel de usuario, y podrá tener interacción con los drivers. No es necesario que escriba las especificaciones del software, pero sí se debe indicar cómo interactuará con los drivers. Cada operación usada debe estar justificada.


Cámara:

            driver_init() {
                //inicializo registro POWER en apagado
                OUT(POWER, 0);
                return DRI_OK;
            }

            int driver_write(int *udata, int size) {
                if(size == sizeof(int)) {
                    int kdata;
                    copy_from_user(&kdata, udata, size);

                    //escribimos en el registro el tiempo que tiene que permanecer prendido
                    OUT(POWER, kdata);
                    return size;
                }
                return DRI_ERROR;
            }

Sensor:

	sem movimiento;
            driver_init() {
                movimiento = sem(0);
                irq_register(6, handler_movimiento);
                return DRI_OK;
            }

            driver_remove() {
                irq_free(6); 
                return DRI_OK;
            }

            void handler_movimiento() {
                movimiento.signal();
            }

            int driver_read(int *udata, int size) {
                if (size == sizeof(int)) {
                    movimiento.wait();

                    int cualquierCosa = 1;      //no importa qué cosa le pase al usuario
                    copy_to_user(udata, cualquierCosa, size);
                    
                    return size;
                }
                return DRI_ERROR;
            }

Software de control:

            int T = 10;
            int main() {
                int camara = open("/dev/camara");
                int sensot = open("/dev/sensor");

                int algoParaElRead;
                while (1) {
                    read(sensor, algoParaElRead, sizeof(algoParaElRead));       //el read es bloqueante si no hay movimiento
                    write(camara, &T, sizeof(T));                               //prendemos por T segundos o resetea el valor en POWER
                }

                return 0;
            }

(c) Explique cómo se genera la interacción a nivel del código entre los drivers que propuso.

>Espero que esté entendible arriba. xd


