# Práctica 4: Administración de memoria

## 1) Fragmentación externa e interna
Explicar la diferencia entre los conceptos de fragmentación interna y externa.
    
>Fragmentación interna: desperdicio de memoria dentro del bloque de memoria asignado (frames o marcos), puede no estar lleno.
>Fragmentación externa: desperdicio de memoria que no se puede usar debido a falla de bloques contiguo y libres.

## 2) Asignación de memoria
Se tiene: - Sistema con 16MB de RAM

- Con particiones de  8MB | 1MB | 4MB | 512KB | 512KB | 2MB
- 5 programas con tamaño: 500KB, 6MB, 3MB, 20KB, 4MB
    
(a) Asignación usando best fit:
>- Asignando 500KB: 8MB | 1MB | 4MB | 12KB | 512KB | 2MB
>- Asignando 6MB:   2MB | 1MB | 4MB | 12KB | 512KB | 2MB
>- Asignando 3MB:   2MB | 1MB | 1MB | 12KB | 512KB | 2MB
>- Asignando 20KB:  2MB | 1MB | 1MB | 12KB | 492KB | 2MB
>- Asignando 4MB:   No se puede 

¿Cuál es la cantidad de bytes de memoria desperdiciados? 

>6MB + 504KB

(b) worst fit:

>- Asignando 500KB: 7MB500KB | 1MB | 4MB | 512KB | 512KB | 2MB
>- Asignando 6MB:   1MB500KB | 1MB | 4MB | 512KB | 512KB | 2MB
>- Asignando 3MB:   1MB500KB | 1MB | 1MB | 512KB | 512KB | 2MB
>- Asignando 20KB:  1MB500KB | 1MB | 1MB | 512KB | 512KB | 1MB980KB
>- Asignando 4MB:   No se puede 

first fit:

>- Asignando 500KB: 7MB500KB | 1MB | 4MB | 512KB | 512KB | 2MB
>- Asignando 6MB:   1MB500KB | 1MB | 4MB | 512KB | 512KB | 2MB
>- Asignando 3MB:   1MB500KB | 1MB | 1MB | 512KB | 512KB | 2MB
>- Asignando 20KB:  1MB480KB | 1MB | 1MB | 512KB | 512KB | 2MB
>- Asignando 4MB:   No se puede 

¿Alguna de las estrategias de asignación vistas en clase (worst fit, first fit) produce como resultado la imposibilidad de ejecutar los 5 programas a la vez?

>Nop

(c) ¿Cuál algoritmo hace el uso más eficiente de la memoria? 

>Ninguna

## 3) Thrashing
Considerar un sistema con paginación por demanda. Tiene los siguientes patrones de uso: 

- Uso de CPU: 20%.
- El sistema hace Thrashing.
- Uso del resto de los dispositivos de E/S: 10%

Indicar si es probable o no que mejore la utilización de la CPU.

>(a) Instalar una CPU más rápida: no mejora, ya que solo se esta usando el 20% del CPU

>(b) Instalar un disco de paginado más grande: mejora, permite asignar páginas sin usar a los procesos, si no roba páginas de otros procesos, se rompe el thrashing.

>(c) Incrementar el grado de multiprogramación: no mejora, tener mas procesos con thrashing genera mas thrashing.

>(d) Disminuir el grado de multiprogramación: mejora, si se solicitan menos páginas.

>(e) Instalar más memoria principal: mejora, se podría pedir mas páginas sin robarle a otro proceso, rompe el thrashing.

>(f) Instalar un disco más rápido: podría mejorar, disminuye el tiempo de carga/descarga de páginas, pero el thrashing persiste.

>(g) Incrementar el tamaño de página: empeora, incrementar el tamaño de la pagina sin aumentar memoria implica menos páginas.

>(h) Incrementar la velocidad del bus de E/S: no mejora, el porcentaje de tiempo de uso de E/S es solo del 10%.

## 4) Page fault
¿Bajo qué circunstancias se produce un page fault? 

>Cuando un proceso va a buscar una página que le corresponde a memoria y no la encuentra
    
¿Cuáles son las acciones que realiza el sistema operativo para resolver la situación?

>1. Encontrar la página deseada en almacenamiento secundario.
>2. Encontrar un frame libre:

>   - Si hay un frame libre, usarlo.
>   - Si no hay un frame libre, usar un algoritmo de reemplazo de páginas para seleccionar un frame vı́ctima.
>   - Escribir el contenido del frame vı́ctima a almacenamiento secundario (si hace falta) (en general, a espacio de swap); actualizar la tabla de páginas (y demás tablas).

>3. Leer la página deseada en el recién liberado frame; actualizar la tabla de páginas (y demás tablas).
>4. Continuar el proceso desde donde ocurrió el page-fault.

## 5) Page Fault Rate
Según la siguiente secuencia de referencias a páginas: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
¿Cuántos fallos de página se producirán con los siguientes algoritmos de reemplazo, suponiendo que se tienen 1, 2, 3, 4, 5, 6 o 7 frames? 
Al comenzar todos los frames se encuentran vacíos.

(a) Con reemplazo LRU.

        (1 frame)
                 frames|orden
        MISS 1 ->    1 | 1
        MISS 2 ->    2 | 2
        ...           ...
		
>Todos miss, total hay 20 fallos de páginas.

        (2 frames)
                  frames|orden
        MISS 1 ->   1 _ | 1 _
        MISS 2 ->   1 2 | 1 2
        MISS 3 ->   3 2 | 2 3
        MISS 4 ->   3 4 | 3 4
        MISS 2 ->   2 4 | 4 2
        MISS 1 ->   2 1 | 2 1
        MISS 5 ->   5 1 | 1 5
        MISS 6 ->   5 6 | 5 6
        MISS 2 ->   2 6 | 6 2
        MISS 1 ->   2 1 | 2 1
         HIT 2 ->   2 1 | 1 2
        MISS 3 ->   2 3 | 2 3
        MISS 7 ->   7 3 | 3 7
        MISS 6 ->   7 6 | 7 6
        MISS 3 ->   3 6 | 6 3
        MISS 2 ->   3 2 | 3 2
        MISS 1 ->   1 2 | 2 1
         HIT 2 ->   1 2 | 1 2
        MISS 3 ->   3 2 | 2 3
        MISS 6 ->   3 6 | 3 6
		
>Total hay 18 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (3 frames)
        MISS 1 -> 1 _ _ | 1
        MISS 2 -> 1 2 _ | 1 2
        MISS 3 -> 1 2 3 | 1 2 3
        MISS 4 -> 4 2 3 | 2 3 4
         HIT 2 -> 4 2 3 | 3 4 2
        MISS 1 -> 4 2 1 | 4 2 1
        MISS 5 -> 5 2 1 | 2 1 5
        MISS 6 -> 5 6 1 | 1 5 6
        MISS 2 -> 5 6 2 | 5 6 2
        MISS 1 -> 1 6 2 | 6 2 1
         HIT 2 -> 1 6 2 | 6 1 2
        MISS 3 -> 1 3 2 | 1 2 3
        MISS 7 -> 7 3 2 | 2 3 7
        MISS 6 -> 7 3 6 | 3 7 6
         HIT 3 -> 7 3 6 | 7 6 3
        MISS 2 -> 2 3 6 | 6 3 2
        MISS 1 -> 2 3 1 | 3 2 1
         HIT 2 -> 2 3 1 | 3 1 2
         HIT 3 -> 2 3 1 | 1 2 3
        MISS 6 -> 2 3 6 | 2 3 6

>Total hay 15 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (4 frames)
        MISS 1 -> 1 _ _ _ | 1
        MISS 2 -> 1 2 _ _ | 1 2
        MISS 3 -> 1 2 3 _ | 1 2 3
        MISS 4 -> 1 2 3 4 | 1 2 3 4
         HIT 2 -> 1 2 3 4 | 1 3 4 2
         HIT 1 -> 1 2 3 4 | 3 4 2 1
        MISS 5 -> 1 2 5 4 | 4 2 1 5
        MISS 6 -> 1 2 5 6 | 2 1 5 6
         HIT 2 -> 1 2 5 6 | 1 5 6 2
         HIT 1 -> 1 2 5 6 | 5 6 2 1
         HIT 2 -> 1 2 5 6 | 5 6 1 2
        MISS 3 -> 1 2 3 6 | 6 1 2 3
        MISS 7 -> 1 2 3 7 | 1 2 3 7
        MISS 6 -> 6 2 3 7 | 2 3 7 6
         HIT 3 -> 6 2 3 7 | 2 7 6 3
         HIT 2 -> 6 2 3 7 | 7 6 3 2
        MISS 1 -> 6 2 3 1 | 6 3 2 1
         HIT 2 -> 6 2 3 1 | 6 3 1 2
         HIT 3 -> 6 2 3 1 | 6 1 2 3
         HIT 6 -> 6 2 3 1 | 1 2 3 6

>Total hay 10 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (5 frames)
        MISS 1 -> 1 _ _ _ _ | 1
        MISS 2 -> 1 2 _ _ _ | 1 2
        MISS 3 -> 1 2 3 _ _ | 1 2 3
        MISS 4 -> 1 2 3 4 _ | 1 2 3 4
         HIT 2 -> 1 2 3 4 _ | 1 3 4 2
         HIT 1 -> 1 2 3 4 _ | 3 4 2 1
        MISS 5 -> 1 2 3 4 5 | 3 4 2 1 5
        MISS 6 -> 1 2 6 4 5 | 4 2 1 5 6
         HIT 2 -> 1 2 6 4 5 | 4 1 5 6 2
         HIT 1 -> 1 2 6 4 5 | 4 5 6 2 1
         HIT 2 -> 1 2 6 4 5 | 4 5 6 1 2
        MISS 3 -> 1 2 6 3 5 | 5 6 1 2 3
        MISS 7 -> 1 2 6 3 7 | 6 1 2 3 7
         HIT 6 -> 1 2 6 3 7 | 1 2 3 7 6
         HIT 3 -> 1 2 6 3 7 | 1 2 7 6 3
         HIT 2 -> 1 2 6 3 7 | 1 7 6 3 2
         HIT 1 -> 1 2 6 3 7 | 7 6 3 2 1
         HIT 2 -> 1 2 6 3 7 | 7 6 3 1 2
         HIT 3 -> 1 2 6 3 7 | 7 6 1 2 3
         HIT 6 -> 1 2 6 3 7 | 7 1 2 3 6

>Total hay 8 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (6 frames)
        MISS 1 -> 1 _ _ _ _ _ | 1
        MISS 2 -> 1 2 _ _ _ _ | 1 2
        MISS 3 -> 1 2 3 _ _ _ | 1 2 3
        MISS 4 -> 1 2 3 4 _ _ | 1 2 3 4
         HIT 2 -> 1 2 3 4 _ _ | 1 3 4 2
         HIT 1 -> 1 2 3 4 _ _ | 3 4 2 1
        MISS 5 -> 1 2 3 4 5 _ | 3 4 2 1 5
        MISS 6 -> 1 2 3 4 5 6 | 3 4 2 1 5 6
         HIT 2 -> 1 2 3 4 5 6 | 3 4 1 5 6 2
         HIT 1 -> 1 2 3 4 5 6 | 3 4 5 6 2 1
         HIT 2 -> 1 2 3 4 5 6 | 3 4 5 6 1 2
         HIT 3 -> 1 2 3 4 5 6 | 4 5 6 1 2 3
        MISS 7 -> 1 2 3 7 5 6 | 5 6 1 2 3 7
         HIT 6 -> 1 2 3 7 5 6 | 5 1 2 3 7 6
         HIT 3 -> 1 2 3 7 5 6 | 5 1 2 7 6 3
         HIT 2 -> 1 2 3 7 5 6 | 5 1 7 6 3 2
         HIT 1 -> 1 2 3 7 5 6 | 5 7 6 3 2 1
         HIT 2 -> 1 2 3 7 5 6 | 5 7 6 3 1 2
         HIT 3 -> 1 2 3 7 5 6 | 5 7 6 1 2 3
         HIT 6 -> 1 2 3 7 5 6 | 5 7 1 2 3 6

>Total hay 7 fallos de páginas

         (7 frames) idéntico arriba ya que entran todas las paginas, solo tienen que cargarse a la memoria ppal
         
>Total hay 7 fallos de páginas

(b) Con reemplazo FIFO.

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (1 frame) Total hay 20 fallos de páginas
        (2 frames) Total hay 18 fallos de páginas
        (3 frames) Total hay 15 fallos de páginas
        (4 frames)
        MISS 1 -> 1 _ _ _ | 1
        MISS 2 -> 1 2 _ _ | 1 2
        MISS 3 -> 1 2 3 _ | 1 2 3
        MISS 4 -> 1 2 3 4 | 1 2 3 4
         HIT 2 -> 1 2 3 4 | 1 2 3 4
         HIT 1 -> 1 2 3 4 | 1 2 3 4
        MISS 5 -> 5 2 3 4 | 2 3 4 5
        MISS 6 -> 5 6 3 4 | 3 4 5 6
        MISS 2 -> 5 6 2 4 | 4 5 6 2
        MISS 1 -> 5 6 2 1 | 5 6 2 1
         HIT 2 -> 5 6 2 1 | 5 6 2 1
        MISS 3 -> 3 6 2 1 | 6 2 1 3
        MISS 7 -> 3 7 2 1 | 2 1 3 7
        MISS 6 -> 3 7 6 1 | 1 3 7 6
         HIT 3 -> 3 7 6 1 | 1 3 7 6
        MISS 2 -> 3 7 6 2 | 3 7 6 2
        MISS 1 -> 1 7 6 2 | 7 6 2 1
         HIT 2 -> 1 7 6 2 | 7 6 2 1
        MISS 3 -> 1 3 6 2 | 6 2 1 3
         HIT 6 -> 1 3 6 2 | 6 2 1 3

>Total hay 14 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
                   
>(5 frames) Total hay 10 fallos de páginas
>(6 frames) Total hay 8 fallos de páginas
>(7 frames) Total hay 7 fallos de páginas

(c) Con reemplazo Second chance.

>(1 frame) Total hay 20 fallos de páginas
>(2 frames) Total hay 18 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (3 frames) 
        MISS 1 -> 1 _ _ | 1
        MISS 2 -> 1 2 _ | 1  2  
        MISS 3 -> 1 2 3 | 1  2  3
        MISS 4 -> 4 2 3 | 2  3  4
         HIT 2 -> 4 2 3 | 2* 3  4
        MISS 1 -> 4 2 1 | 4  2  1
        MISS 5 -> 5 2 1 | 2  1  5
        MISS 6 -> 5 6 1 | 1  5  6
        MISS 2 -> 5 6 2 | 5  6  2
        MISS 1 -> 1 6 2 | 6  2  1
         HIT 2 -> 1 6 2 | 6  2* 1
        MISS 3 -> 1 3 2 | 2* 1  3
        MISS 7 -> 7 3 2 | 3  2  7
        MISS 6 -> 7 6 2 | 2  7  6
        MISS 3 -> 7 6 3 | 7  6  3
        MISS 2 -> 2 6 3 | 6  3  2
        MISS 1 -> 2 1 3 | 3  2  1
         HIT 2 -> 2 1 3 | 3  2* 1
         HIT 3 -> 2 1 3 | 3* 2* 1
        MISS 6 -> 1 6 3 | 3  2  6
        
>Total hay 16 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (4 frames)   
        MISS 1 -> 1 _ _ _ | 1
        MISS 2 -> 1 2 _ _ | 1  2
        MISS 3 -> 1 2 3 _ | 1  2  3
        MISS 4 -> 1 2 3 4 | 1  2  3  4
         HIT 2 -> 1 2 3 4 | 1  2* 3  4
         HIT 1 -> 1 2 3 4 | 1* 2* 3  4
        MISS 5 -> 1 2 5 4 | 4  1  2  5   
        MISS 6 -> 1 2 5 6 | 1  2  5  6
         HIT 2 -> 1 2 5 6 | 1  2* 5  6
         HIT 1 -> 1 2 5 6 | 1* 2* 5  6
         HIT 2 -> 1 2 5 6 | 1* 2* 5  6
        MISS 3 -> 1 2 3 6 | 6  1  2  3
        MISS 7 -> 1 2 3 7 | 1  2  3  7
        MISS 6 -> 6 2 3 7 | 2  3  7  6
         HIT 3 -> 6 2 3 7 | 2  3* 7  6
         HIT 2 -> 6 2 3 7 | 2* 3* 7  6
        MISS 1 -> 6 2 3 1 | 6  2  3  1
         HIT 2 -> 6 2 3 1 | 6  2* 3  1
         HIT 3 -> 6 2 3 1 | 6  2* 3* 1
         HIT 6 -> 6 2 3 1 | 6* 2* 3* 1   
         
>Total hay 10 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (5 frames) 1  2  3  4  4  4  5  6  6  6  6  7  8  8  8  8  8  8  8  8
        
>Total hay 8 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (6 frames) 1  2  3  4  4  4  5  6  6  6  6  6  7  7  7  7  7  7  7  7
        
>Total hay 7 fallos de páginas

        secuencia: 1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6
        (7 frames) 1  2  3  4  4  4  5  6  6  6  6  6  7  7  7  7  7  7  7  7
        
>Total hay 7 fallos de páginas

## 6) Larga secuencias
Suponer una secuencia de referencias a páginas que contiene repeticiones de largas secuencias de referencias a páginas seguidas ocasionalmente por una referencia a una página aleatoria. Por ejemplo, la secuencia: 0, 1, ... , 511, 431, 0, 1, ... , 511, 332, 0, 1, ... consiste en repeticiones de la secuencia 0, 1, ... , 511 seguidas por referencias aleatorias a las páginas 431 y 332.

(a) ¿Por qué los algoritmos de reemplazo LRU, FIFO y Second Chance no serán efectivos para manejar esta dinámica para una cantidad de frames menor al largo de la secuencia?

>Porque se producen MISS constantemente, se van solicitando de forma secuencial cada página y si la cantidad de frames es menor al largo de la secuencia, va a tener que desalojar los primero que luego al dar la vuelta se van a necesitar y así continuamente.

(b) Si este programa tuviera disponibles 500 frames, describir un enfoque de reemplazo de páginas que funcione mejor que los algoritmos LRU, FIFO o Second Chance.

>Un enfoque que funcionaría mejor que LRU, FIFO o Second Chance en este caso es dejar las páginas 1 a 500 fijos en memoria, así se garantiza una gran probabilidad de hit rate. Si se asume que empieza la memoria vacia, empieza con 500 page faults, luego, por cada ciclo solo habrá 11 page faults.

## 7) Second Chance y LRU
Dar un ejemplo simple de una secuencia de referencias a páginas donde la primera página seleccionada para ser reemplazada sea diferente para los algoritmos Second Chance y LRU. Suponer que un proceso tiene tres frames disponibles, y la cadena de referencias contiene números de página del conjunto {0,1,2,3}.

    Secuencia: 1 1 2 3 0
    LRU:
        MISS 1 -> 1 _ _ | 1
         HIT 1 -> 1 _ _ | 1 
        MISS 2 -> 1 2 _ | 1 2 
        MISS 2 -> 1 2 3 | 1 2 3
        MISS 0 -> 0 2 3 | 2 3 0
        
>Se reemplaza la página 1
    
    Second chance:
        MISS 1 -> 1 _ _ | 1
         HIT 1 -> 1 _ _ | 1*
        MISS 2 -> 1 2 _ | 1* 2
        MISS 3 -> 1 2 3 | 1* 2  3
        MISS 0 -> 1 0 3 | 3  1  0

>Se reemplaza la página 2

## 8) Algoritmos similares
"Los algoritmos básicos de reemplazo de página son idénticos, salvo por el atributo utilizado para seleccionar la página a ser reemplazada”.

(a) ¿Qué atributo utiliza el algoritmo FIFO? 

>La página con más tiempo en memoria

¿Y el algoritmo LRU? 

>La página menos usada recientemente

¿Y Second Chance?

>La página con más tiempo en memoria que no haya sido referenciada desde la ultima vez que se chequeo

(b) Pensar el algoritmo genérico para estos algoritmos de reemplazo de páginas.

>Hmmm pensado.

## 9) Desalojo
Una computadora tiene cuatro marcos de página. El tiempo de carga, tiempo de último acceso, y el bit R (referenciado) para cada página están como se muestra en el enunciado.

(a) ¿Qué página reemplazará el algoritmo FIFO?

>Reemplazará la página 3 ya que el tiempo de carga es de 110, el primero que los demás. 

(b) ¿Qué página reemplazará el algoritmo LRU?

>Reemplazará la página 1 ya que el tiempo de ultima referencia es de 265, ubicandose como maxima prioridad de desalojo.

(c) ¿Qué página reemplazará el algoritmo Second Chance?

>Reemplazará la página 2 por no tener el bit de referencia y por llevar mayor cantidad de tiempo. (Cargado en 140)


