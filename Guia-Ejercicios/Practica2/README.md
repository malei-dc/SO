# Práctica 2: Scheduling

## 1) Ráfagas
La siguiente secuencia describe la forma en la que un proceso utiliza el procesador

| Tiempo | Evento |
| :------------ |:---------------:|
| 0 | load store |
| 1 | add store |
| 2 | read de archivo |
| 3 | espera E/S |
| .. | .. |
| 10 | espera E/S |
| 11 | store increment |
| 12 | inc |
| 13 | write en archivo |
| 14 | espera E/S |
| .. | .. |
| 20 | espera E/S |
| 21 | load store |
| 22 | add store |
 
 (a) Identificar las ráfagas de CPU y las ráfagas de E/S.
 
        Ráfagas de CPU: 0-3, 10-14, 20-22
        Ráfagas de E/S: 3-10, 14-20

(b) ¿Qué duración tiene cada ráfaga?

	Las de CPU de 1 unidades de tiempo, y las de E/S 10-3 y 20-14 unidades de tiempo

## 2) Round Robin vs prioridades

- P0 tiene ráfagas cortas de E/S a ciertos dispositivos.
- P1 frecuentemente se bloquea leyendo de la red.
- P2 tiene ráfagas prolongadas de alto consumo de CPU y luego de escritura a disco.

(a) Para planificar estos procesos, ¿convendría usar un algoritmo de Round Robin? ¿convendría usar uno de prioridades? Justifique su respuesta.

        Según cada proceso:
            P0: Rafagas CPU desconocidas - Rafagas E/S cortas
            P1: Rafagas CPU desconocidas - Rafagas E/S largas
            P2: Rafagas CPU largas - Rafagas - Rafagas E/S largas

¿Conviene usar Round Robin?

	- Waiting Time:  
	   - P0 va a estar mucho tiempo esperando, su rafaga de E/S son cortas
	   - P1 no van a estar mucho tiempo esperando, sus rafagas de E/S son largas
	   - P2 va a estar mucho tiempo esperando, su uso del CPU es intentisvo y es posible que lo cortemos por tiempo de quantum
	- Turnaround: 
	   - EL tiempo de ejecucion de P0 y P1 se veran afectados por el tiempo de ejecucion de P2

¿Conviene usar Prioridades?

	Supongamos que asignamos prioridades del estilo P0 > P1 > P2
	   - Mayor prioridad a P0 porque tiene Rafagas E/S cortos, por lo que va a dejar mas tiempo a los demas procesos
	   - Menor prioridad a P2 porque tiene Rafagas de CPU largas y generaria mayor wait-time a los demas procesos

	- Waiting Time:
	   - P0: Ningun tiempo de espera debido a que es el de maxima prioridad
	   - P1: Deberia tener poco tiempo de espera ya que P0 tiene rafagas E/S cortas
	   - P2: Mayor tiempo de espera por tener menos prioridad que los otros 2

	- Turnaround:
	   - P0: Bajo por tener maxima prioridad.
	   - P1: Bajo por tener mucho periodo de ejecucion debido a las cortas rafagas de E/S de P0
	   - P2: Largo por tener baja prioridad.

	Conclusión: siempre que haya diferencias considerables en frecuencia de ráfagas/procesos mas importantes que otros, conviene usar prioridades.
	La desventaja es que si continuamente se reciben procesos del maximo prioridad, se produce starvation.

## 3) Tipo de Scheduler
¿A qué tipo de scheduler corresponde el siguiente diagrama de transición de estados de un proceso? [Gráfico en el enunciado](https://github.com/malei-dc/SO/blob/main/Guia-Ejercicios/Practica2/practica2.pdf)

	Representa a un scheduler nonpreentive ya que tiene el estado blocked y no el waiting. Uno preentive dejaria al proceso que saca en estado waiting.

## 4) Inanición - starvation
¿Cuáles de los siguientes algoritmos de scheduling pueden resultar en starvation (inanición) y en qué condiciones?

- Round-robin: no tiene starvation.
- Por prioridad: tiene starvation cuando llega continuamente procesos de maxima prioridad.
- SJF (shortest job first): tiene starvation cuando llega continuamente procesos cortos de poco tiempo.
- SRTF (shortest remaining time first): idem arriba.
- FIFO: no hay starvation.
- Colas de multinivel: tiene starvation cuando llega constantemente procesos con alta prioridad.
- Colas de multinivel con feedback (aging): no hay starvation.

## 5) Modificación a round-robin
Considere una modificación a round-robin en la que un mismo proceso puede estar encolado varias veces en la lista de procesos ready. 
Por ejemplo, en un RR normal se tendrían en la cola ready a P1,P2,P3,P4, con esta modificación se podría tener P1,P1,P2,P1,P3,P1,P4.

(a) ¿Qué impacto tendría esta modificación?

	Si se apilan el mismo proceso seguido termina pareciendo a fifo. 

(b) Dar ventajas y desventajas de este esquema.

	Desventaja: la distribución de CPU es menos equitativa en cada proceso, mas turnarround, waiting time y response time que RR normal.
	Ventaja: menos cambios de contexto, mejor tiempo de respuesta a los proceso encolados varias veces.
    
(c) ¿Se le ocurre alguna otra modificación para mantener las ventajas sin tener que duplicar las entradas en la lista de procesos ready?

	Asignar quantum diferentes segun prioridad podría ser una opción.

## 6) Diagramas de Gantt
Llegan en el orden P1, P2, P3, P4, P5 en el instante 0.

| Proceso | Ráfaga de CPU | Prioridad |
|:---------:|:---------:|:---------:|
| P1 | 10 | 3
| P2 | 1 | 1 |
| P3 | 2 | 3 |
| P4 | 1 | 4 |
| P5 | 5 | 2 | 

(a) Dibujar los diagramas de Gantt para ilustrar la ejecución de estos procesos usando los algoritmos de scheduling FCFS, SJF, con prioridades sin desalojo (a menor el número, mayor la prioridad), round-robin (quantum de 1 unidad de tiempo, ordenados por el número de proceso).

FCFS: no nos importa la prioridad, solo el orden de llegada

| | | P1| P2 | P3 | P4 | P5 |
|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 0 | 10 | 11 | 13 | 14 | 19 | 

        waiting time promedio: (0 + 10 + 11 + 13 + 14) / 5 = 9,6
        turnarround promedio: (10 + 11 + 13 + 14 + 19) /5 = 13,4

SJF: no nos importa prioridad, solo el tiempo.

| | | P2| P4 | P3 | P5 | P1 |
|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 0 | 1 | 2 | 4 | 9 | 19 | 

        waiting time promedio: (0 + 1 + 2 + 4 + 9) / 5 = 3,2
        turnarround promedio: (1 + 2 + 4 + 9 + 19) / 5 = 7

Prioridades sin desalojo (a menor el número, mayor la prioridad)

| | | P2| P5 | P1 | P3 | P4 |
|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 0 | 1 | 6 | 16 | 18 | 19 | 

        waiting time promedio: (0 + 1 + 6 + 16 + 18) / 5 = 8,2
        turnarround promedio: (1 + 6 + 16 + 18 + 19) / 5 = 12

RR (quantum de 1 unidad de tiempo)
         
| | | P1 | P2 | P3 | P4 | P5 | P1 | P3 | P5 | P1 | P5 | P1 | P5 | P1 | P5 | P1 | P1 | P1 | P1 | P1 |
|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 |

        waiting time promedio: (9 + 1 + 5 + 3 + 9) / 5 = 5,4
        turnarround promedio: (19 + 2 + 7 + 4 + 14) / 5 = 9,2

(b) ¿Cuál es el waiting time promedio y de turnaround promedio para cada algoritmo?
-Completado arriba-
    
(c) ¿Cuál de los algoritmos obtiene el menor waiting time promedio, y el menor turnaround?

        El menor en ambos fue SJF.

## 7) Dado un Gantt
| Proceso | Ráfaga de CPU | Instante de llegada (ms) |
|:---------:|:---------:|:---------:|
| P1 | 3 | 0 |
| P2 | 6 | 2 |
| P3 | 4 | 4 |
| P4 | 5 | 6 |
| P5 | 2 | 8 | 

| | | P1 | P2 | P3 | P5 | P2 | P4 |
|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 0 | 3 | 4 | 8 | 10 | 15 | 20 | 

(a) Calcular el waiting time y el turnaround promedios.

        waiting time promedio: (0 + 7 + 0 + 9 + 0) / 5 = 3,2
        turnarround promedio: (3 + 13 + 4 + 14 + 2) / 5 = 7,2
    
(b) Indicar de qué tipo de scheduler se trata, justificando claramente esa conclusión.

	Es un scheduler con desalojo ya que P2 fue desalojado sin haber terminado al momento que llega P3.
	Posiblemente con prioridades ya que P2 no siguió hasta que haya terminado P5. Si este fuera el caso, P3 y P5 tendrían mayor prioridad que P2 y P4. No tengo suficiente dato para conjeturar sobre P1, ni tampoco saber quien tiene mayor prioridad entre P3 y P5 ni entre P2 y P4.

## 8) Dado una tabla de tiempos
| Proceso | Ráfaga de CPU | Instante de llegada |
|:---------:|:---------:|:---------:|
| P1 | 8 | 0.0 |
| P2 | 4 | 0.4 |
| P3 | 1 | 1.0 |

(a) ¿Cuál es el tiempo de turnaround promedio para estos procesos usando FCFS?

	FCFS -> turnaround promedio: (8 + 11,6 + 11) / 3 = 10,2 

(b) ¿Cuál es usando SJF?

	SJF -> turnaround promedio: (8 + 11,6 + 11) / 3 = 10,2

(c) SJF se supone que mejora la perfomance, pero al elegir ejecutar P1 inicialmente no había forma de saber que iban a llegar dos cortos luego. Volver a calcular el tiempo de turnaround promedio pero dejando el procesador idle por una unidad de tiempo y luego usar SJF.

	SJF (dejando idle 1 u de tiempo) -> turnaround promedio: (1 + 4,6 + 14) / 3 = 6,53333

## 9) Más Gantt - caso particular
Para los procesos presentados en la siguiente tabla, realizar un gráfico de Gantt para cada uno de los algoritmos de scheduling indicados:
- FCFS
- RR (quantum=10)
- SJF

| Proceso | Ráfaga de CPU | Instante de llegada |
|:---------:|:---------:|:---------:|
| P1 | 1 | 5 |
| P2 | 10 | 6 |
| P3 | 1 | 7 |
| P4 | 10 | 8 |

Calcular el waiting time y el turnaround promedios para cada una de los algoritmos.

- FCFS:

| | | P1 | P2 | P3 | P4 |
|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 5 | 6 | 16 | 17 | 27 | 

          waiting time promedio: (0 + 0 + 9 +9) / 4 = 4,5
          turnaround promedio: (1 + 10 + 10 + 19) / 4 = 10

- RR(10 quantum): igual FCFS

- SJF: igual FCFS

## 10) Más Gantt - FF vs SRF
Considere los siguientes procesos:

| Proceso | Ráfaga de CPU | Instante de llegada |
|:---------:|:---------:|:---------:|
| P1 | 8 | 0 |
| P2 | 8 | 5 |
| P3 | 6 | 14 |
| P4 | 5 | 15 |

(a) Diagrama de Gantt para un algoritmo de scheduling round-robin con un quantum de 5 unidades de tiempo.

RR:
      
| | | P1 | P2 | P1 | P2 | P3 | P4 | P3 |
|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 0 | 5 | 10 | 13 | 16 | 21 | 26 | 27 |

       turnaround promedio: (13 + 11 + 13 + 11) / 4 = 12

(b) Diagrama de Gantt para un algoritmo tipo shortest remaining time first.
SRF:

| | | P1 | P2 | P4 | P3 |
|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 0 | 8 | 16 | 21 | 27 |

        turnaround promedio: (8 + 11 + 13 + 6) / 4 = 9,5
    
(c) Calcular el tiempo de turnaround promedio en ambos casos.
-Turnaround promedio hecho arriba.-

(d) Explicar por qué en algunos contextos podría tener sentido utilizar RR
 - Es costoso calcular el tiempo restante.
 - Los procesos interactivos tienen que ser fluidos, si llegan muchos procesos con poca rafaga, starvation, mejor RR

## 11) Gantt Multilevel feedback queue dos Colas
Considere los siguientes procesos:

| Proceso | Ráfaga de CPU | Instante de llegada | Cola asignada |
|:---------:|:---------:|:---------:|:---------:|
| P1 | 4 | 0 | 1 |
| P2 | 3 | 0 | 1 |
| P3 | 8 | 0 | 2 |
| P4 | 5 | 10 | 2 |

(a) Realizar un diagrama de Gantt para un algoritmo de scheduling Multilevel feedback queue con dos colas: 

- Cola 1 con quantum de 1 unidad de tiempo  (Asumo RR)
- Cola 2 con FCFS. 

La cola 1 tiene más prioridad que la 2. Usa política con desalojo. Para cada proceso se indica qué cola se le asigna en el momento de su llegada.

Cola 1 RR: 

| | | P1 | P2 | P1 | P2 | P1 | P2 | P1 |
|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 

Cola 2 FCFS: 

| | | P3 | P4 |
|:---------:|:---------:|:---------:|:---------:|
| Tiempo | 7 | 15 | 20 |

(b) Calcular el tiempo de turnaround promedio y el waiting time promedio. 

	Waiting time promedio: (3 + 3 + 7 + 5) / 4 = 4,5
	Turnaround promedio: (7 + 6 + 15 + 10) / 4 = 9,5

## 12) Sistema que favorece menos uso de CPU
- Explique por qué favorecería a los procesos que realizan muchas E/S: Los procesos que realizan muchas E/S utilizan el procesador en rafagas cortas y lo liberan en poco tiempo hasta estar en ready de vuelta. Estos procesos se verian beneficiados ya que el algoritmo los consideraria prioritarios por tener rafagas cortas.

- Explique por qué no dejaría a los intensivos en CPU en starvation: Porque de tanto estar sin usar el CPU pasarian a ser el proceso que MENOS USO el CPU en el ultimo tiempo, ganando prioridad.

## 13) Procesos cortos
Explicar cómo los siguientes algoritmos favorecen (o desfavorecen) a los trabajos más cortos:

- (a) FIFO: desfavorecen trabajos cortos, si un proceso largo llega primero, los cortos se quedan esperando.

- (b) RR: favorecen, le otorga un quantum equitativo a los procesos, dandole más oportunidad de terminar a los procesos cortos.

- (c) Multilevel feedback queue: favorecen, generalmente los procesos cortos que usan menos CPU son mas prioritarios.

## 14) Sistema bancario
Un sistema que atiende tareas interactivas de varias sucursales bancarias está conectado en forma directa a la central policial y, frente a un caso de robo, genera un proceso que activa una alarma en la central.

(a) Diseñar un algoritmo que permita que, una vez generado ese proceso de alarma, tenga prioridad sobre el resto de las tareas

	El sistema tendrá un Multilevel queue donde habrá dos colas: 
	   -Cola 1: FCFS para las alarmas.
	   -Cola 2: RR para tareas interactivas.
	  
	La cola 1 tiene prioridad mayor a la cola 2, por lo tanto si llega algún proceso de alarma, se pausa cualquier otra tarea. Se asume que las alarmas se atienden a la brevedad debido a la urgencia, aunque se pueda generar distintas alarmas de distintas sucursales, se van atendiendo por orden de llegada, por eso FCFS. En cambio, para las tareas interactivas, se busca una mayor fluidez posible, para esto es ideal manejarse en una cola RR. No hay aging.

## 15) RR vs FCFS
Se tiene un sistema donde hay trabajos interactivos y de procesamiento de datos. Los de procesamiento de datos leen archivos inmensos, hacen pequeñas cuentas y los vuelven a grabar.
Se desea que los usuarios interactivos tengan la sensación de buen tiempo de respuesta, pero sin perjudicar excesivamente el throughput del sistema.
Round-robin o con FCFS. ¿Qué política utilizaría y por qué? Justificar 

	FCFS: va atendiendo los procesos por orden de llegada, si llegan primero los procesos de datos que leen archivos inmensos, va a perjudicar en el throughput de los procesos interactivos, también dificulta la sensacion de fluidez que necesita los procesos interactivos.

	RR: va repartiendo quantum a todos los procesos en estado ready, permite que los procesos interactivos vayan terminando, lo que genera mejor tiempo de respuesta. El throughput no se veria afectado negativamente por la razón mencionada anteriormente.

	Lo mas ideal sería RR.

## 16) Seriografía
Se tienen entonces los siguientes procesos:

- Uno que genera las imágenes digitales a partir de los valores resultantes al irradiar al paciente
- Uno que responde a los botones de ajuste de brillo y contraste
- Uno que responde a los botones de ajuste de zoom

¿Qué política de scheduling permite esta toma y edición de imágenes “en vivo” de manera eficiente? Justificar.

	Propuesta: una política con prioridades con aging. Puede ser FCFS ya que nos interesa el orden de llegada de los procesos.
	   -La máxima prioridas lo tiene los procesos de generación de imagenes
	   -Los botones de brillo y zoom tienen la misma prioridad. Para mejorar el tiempo de respuesta, propongo que suba de prioridad cuando haya pasado determinada cantidad de tiempo.

##17) Sistema de vigilancia
Modulos:

- Procesamiento de video son procesos que leen el stream de video que llega desde una cámara y luego corren un algoritmo de detección de objetos. Para este tipo de procesos se quiere evitar situaciones de scheduling poco "justas".
- Sistema debe alertar a los operadores para que actúen de inmediato.
- Periódicamente se lanzan procesos que levantan grandes volúmenes de video grabados durante el día y le aplican un algoritmo de compresión que permite luego reemplazar las secuencias originales por otras de mucho menor tamaño. 
---
	Politica de scheduler: Multilevel queue
	Cola 1: FCFS, para procesos de alerta al operador.
	Cola 2: RR, para procesos de detección de objetos.
	Cola 3: RR, procesos de compresión.
	
	Prioridades: Cola 1 > Cola 2 > Cola 3.
	
	Justificación: 
	   - Las alarmas tienen ráfagas de CPU cortas y necesitan atenderse lo más urgentemente posible.
	   - La ráfaga de procesamiento de video tiene duración variante según la hora,  por lo que es importate RR para dividir tiempo de CPU de manera justa.
	   - La compresión se hace solo de noche y tiene ráfaga de CPU grande, sería un proceso batch.
