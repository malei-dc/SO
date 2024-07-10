#Práctica 3: sincronización entre procesos

## 1) Analizando código
A continuación se muestran dos códigos de procesos que son ejecutados concurrentemente.
La variable X es compartida y se inicializa en 0.

<ul><li> Proceso A:  </li></ul>

	X = X + 1;
	printf("%d", X);

 <ul><li> Proceso B:  </li></ul>
 
	X = X + 1;

Las variables X e Y son compartidas y se inicializan en 0.

<ul><li> Proceso A:  </li></ul>

	for (; X < 4; X++) {
	   Y = 0;
	   printf("%d", X);
	   Y = 1; }

 <ul><li> Proceso B:  </li></ul>
 
	 while (X < 4) {
	   if (Y == 1)
	      printf("a"); }

No hay información acerca de cómo serán ejecutados por el scheduler para ninguno de los dos.

a) ¿Hay una única salida en pantalla posible para cada código?
    
>Primer caso:
>No hay una única salida. (Asumo que se ejecutan una sola vez) 
>Posibles: 1, 2
>-- -- 
>Segundo caso:
>Hay una salida posible. El proceso A crashea ya que no hay inicialización para el X en el for.
>Posible: indefinido, cuelga el proceso B.

b) Indicar todas las salidas posibles para cada caso.
-Completado arriba-

## 2) Race condition
Se tiene un sistema con cuatro procesos accediendo a una variable compartida x y un mutex, el siguiente código lo ejecutan los cuatro procesos. 

    x = 0; // Variable compartida
    mutex(1); // Mutex compartido
    while (1) {
        mutex.wait();
        y = x; // Lectura de x
        mutex.signal();
        if (y <= 5) {
            x++;
        } else {
            x--;
        }
    }

Se debe asegurar que cada vez que un proceso lee de la variable compartida,
previamente solicita el mutex y luego lo libera. ¿Estos procesos cumplen con lo planteado? ¿Pueden ser víctimas de race condition?

>Cumple con lo planteado con las variables compartidas y el mutex. Pero hay race conditions ya que se acceden a las variables compartidas fuera del mutex. Pero como no se imprime nada por pantalla, no devuelve nada, lo que puede pasar es que en c/proceso alguna variable pueden valer distintos.

## 3) Wait()
La operación wait() sobre semáforos suele utilizar una cola para almacenar los pedidos que se encuentran en espera. Si en lugar de una cola utilizara una pila (LIFO), determinar si habría inanición o funcionaría correctamente.

>Si wait usara cola LIFO se afectaría FAIRNESS, ya que en el caso en el que llega procesos constantemente, los procesos mas viejos no reciben  turno, por ende también afecta el WAIT-FREEDOM. EXCL se mantiene.

## 4) Funciones no atomico
Demostrar que, en caso de que las operaciones de semáforos wait() y signal() no se ejecuten atómicamente, entonces se viola la propiedad EXCL.

    wait(): while (s<=0){dormir();}
            s--;

    signal(s): s++;
               if(alguien espera por s) {despertarUno();}

>Imaginemos que usamos un semaforo que controla la suma de una variable compartida no atomica entre dos procesos. Si el semaforo se inicializa en 1, y supongamos que ambos procesos ejecutan el wait al mismo tiempo por primera vez, como no es atomico el wait(), le permitiría a ambos procesos acceder a la zona crítica, modificando la variable y repercutiendo en la correctitud de la operación.

## 5) Starvation
Se tienen n procesos, se espera que todos los procesos terminen de ejecutar la función preparado() antes de que alguno de ellos llame a la función critica().

    preparado()
    mutex.wait()
    count = count + 1
    mutex.signal()
    if (count == n)
        barrera.signal()
    barrera.wait()
    critica()

¿Por qué la solución permite inanición?
>Hay dos problemas. 

>Primero: si el proceso n es desalojado despues  del mutex y antes de la comparacion del count, y viene otro proceso sumando count 1, el programa nunca termina ya que nunca se libera la barrera.
    
>Segundo: cuando libera la primera barrera, no libera el siguiente, asi que habría n-1 procesos trabados en barrera.wait().

Modificar el código para arreglarlo.

    preparado()
    mutex.wait()
    count = count + 1
    if (count == n)
        barrera.signal()
    mutex.signal()
    barrera.wait()
    barrera.signal()
    critica()

## 6) Variables atómicas vs semáforos
Cambie su solución del ejercicio anterior con una solución basada solamente en las herramientas atómicas.

    preparado()
    count.getAndInc()
    while(true){
        if (count == n)
            critica()
            break
    }

(a) ¿Cuál de sus dos soluciones genera un código más legible? 

>Para mí el de variable atomicas.

(b) ¿Cuál de ellas es más eficiente? ¿Por qué? 

>El de semáforos, porque no quedan loopeando (gastar CPU sin hacer nada), si no que se bloquean esperando.

(c) ¿Qué soporte requiere cada una de ellas del SO y del HW? 

>Solución atómica requiere la implementación de las operaciones atómicas (proveidas por el HW)

>Solución de semáforos requiere su implementación por el sistema operativo, ya que es necesario que el scheduler participe en el proceso de despertado de procesos

## 7) Sincronización de n procesos
Se tiene varibles n, i, donde son n procesos, y se quiere que ejecute Pi , Pi+1 , ..., PN −1 , P0 , ..., Pi−1.

    //inicialización de estructura de sincronización
    semaforos[n]
    for (int j = 0; j < n; j++) {
        if (j == i) 
            semaforos[j] = sem(1);
        else 
            semaforos[j] = sem(0);
    }

    //procesos
    P(i): semaforos[i].wait()
            codigo();
          semaforos[(i + 1) % n].signal()

## 8) Semáforos
Considere cada uno de los siguientes enunciados, para cada caso, escriba el código que permita la ejecución de los procesos según la forma de sincronización planteada. Debe argumentar porqué cada solución evita la inanición:

(a) Se tienen tres procesos (A, B y C). Se desea que el orden en que se ejecutan sea el orden alfabético, es decir que las secuencias normales deben ser: ABC, ABC, ABC, ...

	//inicialización de estructuras de sincronización
	permisoA = sem(1)
	permisoB = sem(0)
	permisoC = sem(0)

	//procesos (asumo que se ejecutan muchas veces)
	A: permisoA.wait()                                        
	     codigoA()                                               
	   permisoB.signal()                             

	B: permisoB.wait()
	     codigoB() 
	   permisoC.signal()  

	C: permisoC.wait() 
	     codigoC()
	   permisoA.signal()

>¿Hay inanición? No. Esta solución evita inanición ya que el semaforo que deja pasar el proceso A se inicializa en 1, y al finalizar despierta el semaforo del proceso B haciendo lo mismo para el semáforo de C, siempre va a haber un proceso activo y dos esperando a la señal. 

(b) Idem anterior, pero se desea que la secuencia normal sea: BBCA, BBCA, BBCA, ...

	//inicialización de estructuras de sincronización
	permisoA = sem(0)
	permisoB = sem(1)
	permisoC = sem(0)

	//procesos (asumo que se ejecutan muchas veces)
	A: permisoA.wait()
	      codigoA()
	   permisoB.signal()

	B: permisoB.wait()
	      codigoB()  
	      codigoB()
	   permisoC.signal()

	C: permisoC.wait()
	      codigoC()
	   permisoA.signal()

>¿Hay inanición? No. La razón por la que no hay inanición es muy similar que la anterior, solo que esto ejecuta el código de B dos veces e inicializo el semaforo del proceso B en 1 para que empiece.

(c) Se tienen un productor (A) y dos consumidores (B y C) que actúan no determinísticamente. La información provista por el productor debe ser retirada siempre 2 veces, es decir que las secuencias normales son: ABB, ABC, ACB o ACC.

	//inicialización de estructuras de sincronización y variables compartidas
	productor = sem(1)
	consumidor = sem(0)
	mutex = sem(1)
	int contador = 0

	//procesos (asumo que se ejecutan muchas veces)
	A: productor.wait()
	   mutex.wait()
	   contador = 0;
	   mutex.signal()
	      codigoA()
	   consumidor.signal()
	   consumidor.signal()

	B: consumidor.wait()
	   codigoB()
	   mutex.wait()
	   contador ++
	   if (contador == 2)
	      productor.signal()
	   mutex.signal()

	C: consumidor.wait()
	   codigoC()
	   mutex.wait()
	   contador ++
	   if (contador == 2)
	      productor.signal()
	   mutex.signal()

>¿Hay inanición? No, misma razón que arriba, ya que acá solo se agrega un mutex para la que la manipulación del contador sea en 1 proceso a la vez.

(d) Se tienen un productor (A) y dos consumidores (B y C). Cuando C retira la información, la retira dos veces. Los receptores actúan en forma alternada. Secuencia normal: ABB, AC, ABB, AC, ABB, AC...

	//inicialización de estructuras de sincronización y variables compartidas
	productor = sem(1)
	consumidorB = sem(0)
	consumidorC = sem(0)
	mutex = sem(1)
	int contador = 0 (solo en proceso A)

	//procesos (asumo que se ejecutan muchas veces)
	A: productor.wait()
	   condigoA()
	   if(contador % 2 == 0)
	      consumidorB.signal()
	   else
	      consumidorC.signal()
	   contador++

	B: consumidorB.wait()
	   codigoB()
	   codigoB()
	   productor.signal()

	C: consumidorC.wait()
	   codigoC()
	   productor.signal()

>¿Hay inanición? No. Las estructuras de sincronización garantiza que siempre haya solo un proceso ejecutando, empieza permitiendo con el productor donde tiene un contador que solo accede ahí, según si es par o impar despierta el consumidor B o C. Cada consumidor cuando despierta ejecuta su código las veces que corresponde y le devuelve el "turno" al productor.

## 9) N procesos
Suponer que se tienen N procesos $$P_i$$ , cada uno de los cuales ejecuta un conjunto de sentencias $$a_i$$ y $$b_i$$ .  ¿Cómo se pueden sincronizar estos procesos de manera tal que los $$b_i$$ se ejecuten después de que se hayan ejecutado todos los $$a_i$$?

>Si tengo la variable N como dato de entrada, puedo definir un contador compartido atómico entre todos los procesos y cuando llegue a N, desbloquear la barrera en cadena. Las estructuras de sincronización necesarias será un semaforo de permiso que funciona como barrera y un mutex. El codigo de cada proceso consiste en que se ejecute $$a_i$$ antes de la barrera y $$b_i$$ despues de la barrera, los $$b_i$$ empiezan a ser accesible una vez todos los procesos hayan ejecutado $$a_i$$.