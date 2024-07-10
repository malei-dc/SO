# Práctica 3: sincronización entre procesos

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
Suponer que se tienen N procesos $P_i$ , cada uno de los cuales ejecuta un conjunto de sentencias $a_i$ y $b_i$ .  ¿Cómo se pueden sincronizar estos procesos de manera tal que los $b_i$ se ejecuten después de que se hayan ejecutado todos los $a_i$?

>Si tengo la variable N como dato de entrada, puedo definir un contador compartido atómico entre todos los procesos y cuando llegue a N, desbloquear la barrera en cadena. Las estructuras de sincronización necesarias será un semaforo de permiso que funciona como barrera y un mutex. El codigo de cada proceso consiste en que se ejecute $a_i$ antes de la barrera y $b_i$ despues de la barrera, los $b_i$ empiezan a ser accesible una vez todos los procesos hayan ejecutado $a_i$.

## 10) foo y bar
Dos procesos, comparten los semáforos S y R, ambos inicializados en 1, y una variable global x, inicializada en 0.

    void foo( ) {
        do {
            semWait(S);
            semWait(R);
            x++;
            semSignal(S);
            semSignal(R);
        } while (1);
    }

    void bar( ) {
        do {
            semWait(R);
            semWait(S);
            x--;
            semSignal(S);
            semSignal(R);
        } while (1);
    }

(a)¿Puede alguna ejecución de estos procesos terminar en deadlock? En caso afirmativo, describir una traza de ejecución

>Si puede terminar en deadlock debido al orden de los waits del principio de cada proceso. Suponer que se ejecutan los dos procesos al mismo tiempo la primera linea, foo pone wait a S y bar pone en wait a R, ambos que quedan trabados en la segunda linea. 

(b) ¿Puede alguna ejecución de estos procesos generar inanición para alguno de los procesos? En caso afirmativo, describir una traza.
    
>No hay inanición, si foo dispara el signal R, incluso si bar pierde el sem S, está garantizado de conseguirlo en algún momento y vice-viceversa.

## 11) Problema de los prisioneros

P prisioneros están encarcelados. Para salir de prisión se les propone el siguiente problema.
- Los prisioneros tienen un día para planear una estrategia. Después, permanecerán en celdas aisladas sin ninguna comunicación.
- Hay una sala con una luz y un interruptor. La luz puede estar prendida (interruptor on) o apagada (interruptor off ).
- De vez en cuando, un prisionero es llevado a esa sala y tiene derecho cambiar el estado del interruptor o dejarlo como está.
- Se garantiza que todo prisionero va a entrar a la sala infinitas veces.
- En cualquier momento, cualquier prisionero puede declarar “todos los prisioneros hemos visitado la sala al menos una vez".
- Si la declaración es correcta, los prisioneros serán liberados. Si no, quedarían encerrados para siempre.

Definir una estrategia que permita liberar a los prisioneros sabiendo que el estado inicial del interruptor es off (luz apagada) y considerando que no todos los prisioneros tienen por qué hacer lo mismo en cada momento.

    void PrisioneroCero() {
    contador = 0;
        while (!libres) {
            // Esperar que la sala esté libre
            ...
            if (!luz) {
                luz = true
                contador++
                if (contador == N) libres = true;
            }
        }
    }

    // Resto de los prisioneros
    void RestoDeLosPrisionero(int i) {
        entreASala = false;
        while (!libres) {
            // Esperar que la sala esté libre
            ...
            if (luz && !entreASala) {
                entreASala = true
                luz = false;
            }
        }
    }

(a) Defina el tipo de las variables utilizadas (considere además los tipos atómicos) para el correcto funcionamiento de la solución.

>La unica variable necesaria para el correcto funcionamiento del ejercicio es un semaforo que restringa el acceso a la sala
Si no existe, multiples prisioneros pueden entrar a la sala al mismo tiempo para cambiar el valor de la bombilla
Si 2 prisioneros alteran la bombilla antes de que la vea el prisionero 0, entonces se arruina el calculo para liberarlos
La variable entreALaSala no es necesario que sea atomica, solo un proceso va a tocarla (alguno de los prisioneros restantes que no sean 0)
La variable contador no es necesario que sea atomica, solo un proceso va a tocarla (el prisionero 0)

(b) Modificar el código de los prisioneros para que entren a la sala de a uno por vez. Explicar porqué su solución garantiza la exclusión mutua sobre la sala

Codigo:

            semSala = 1

            void PrisioneroCero(){
                contador = 0;
                while (!libres) {
                    // Esperar que la sala esté libre
                    ...
                    semSala.wait()
                    if (!luz){
                        luz = true
                        contador++
                        if(contador == N) libres = true;
                    }
                    semSala.signal()
                }
            }

            void RestoDeLosPrisionero(int i){
                entreASala = false;
                while (!libres) {
                    // Esperar que la sala esté libre
                    ...
                    semSala.wait()
                    if (luz && !entreASala){
                        entreASala = true
                         luz = false;
                    }
                    semSala.signal()
                }
            }

>El semaforo TIENE que estar por fuera del if, sino existe casos en el que un prisionero queda adentro con la luz apagada
Ejemplo con P1, P2 que no entraron a la sala:
- P1 entra a la sala, se apropia del semaforo y el scheduler lo interrumpe cuando setea entreASala en true
- P2 llega, ve que la luz esta prendida y entra a la sala, se queda esperando en el semaforo que tomo P1
- P1 vuelve a ejecutar, apaga la luz y manda signal al semaforo
- P2 se despierta, apaga la luz apagada y se va de la sala
- P0 observa que la luz esta apagada y solo cuenta 1 (pero 2 prisioneros estuvieron en la sala)
        
>Es necesario usar el semaforo en el prisionero 0 ya que si no, el scheduler puede cortar el prisionero 0 luego de poner luz en true y antes de sumar 1 al contador, permitiendo pasar en total 2 prisionero y al volver el proceso del prisionero 0 contar solo 1.

## 12) Read y Write
Se quiere simular la comunicación mediante pipes entre dos procesos usando las syscalls read() y write(), pero usando memoria compartida (sin usar file descriptors). Se puede pensar al pipe como si fuese un buffer de tamaño N, donde en cada posición se le puede escribir un cierto mensaje. El read() debe ser bloqueante en caso que no haya ningún mensaje y si el buffer está lleno, el write() también debe ser bloqueante. No puede haber condiciones de carrera y se puede suponer que el buffer tiene los siguientes métodos: pop() (saca el mensaje y lo desencola), push() (agrega un mensaje al buffer).

    //inicialización de estructuras de sincronización y variables compartidas
    buffer = cola(n)
    mutex = sem(1)
    int cont = 0
    semRead = sem(0)
    semWrite = sem(1)

    //procesos
    read:
        mutex.wait()
        if (cont == 0)
            mutex.signal()
            semRead.wait()
        else 
            buffer.pop()
            cont --
            if (cont == N - 2)
                semWrite.signal()
            mutex.signal()
    
    write(msg):
        mutex.wait()
        if (cont == N - 1)
            mutex.signal()
            semWrite.wait()
        else
            buffer.push(msg)
            cont ++
            if (cont == 1)
                semRead.signal()
            mutex.signal()

## 13) Game Pool Party!
Se encuentran N mesas con capacidad para 4 personas. Los clientes entran constantemente y van ocupando las mesas hasta llenar su capacidad, preferentemente aquellas que ya tienen clientes en la mesa. Cuando recien una mesa tiene 4 personas, cada cliente debe invocar la función jugar(). Cuando las N mesas están ocupadas, los nuevos clientes tienen que esperar a que se libere alguna mesa. Las mesas solo se liberan todas juntas, es decir, los clientes terminan de jugar y abandonan la mesa.
Suponga que tiene funcines:

- conseguirMesa(): devuelve el número de mesa que el cliente tiene que ir. La función es bloqueante cuando ya no hay más mesas libres. 
- abandonarMesa(i): se llama cuando los jugadores dejan la mesa, donde i es el número de mesa actual.
---
**Solución incorrecta: (no modela bien clientes)**

	//Estructuras de sincronización y variables compartidas
	int personas = 0
	int mesasOcupadas = 0

	mutex = sem(1)
	semBar = sem(0)
	mesas[N]
	for (int i = 0; i < N; i++) {
		mesas[i] = sem(0)
	}

	//Código
	bar():
		personas ++
		if (personas >= 4)
			persona = personas - 4
			mutex.wait()
		if(mesasOcupadas == N)
			mutex.signal()
			semBar.wait()
		else
			int destino = conseguirMesa()
			mesas[destino].signal()
			mesasOcupadas ++
			mutex.signal()

	mesa(i):
		mesas[i].wait()
		jugar()
		abandonarMesa(i)
		mutex.wait()
		if (mesasOcupadas == N)
			mesasOcupadas --
			semBar.signal()
			mutex.signal()
		else 
			mesasOcupadas --
			mutex.signal()

La principal razón de que esta solución esté mal es que no cumple que *las mesas solo se liberan todas juntas* , es decir, todas las personas de la mesa terminan de jugar y se levantan a la vez.

**Solución correcta:**

    //Estructuras de sincronización y variables compartidas
    vector<int> cantidadJugadores = vector<int>(N, 0)
    barreraJugar[N]
    barreraSalida[N]
    mutex[N]
    for (int i = 0; i < N; i++) {
        barreraJugar[i] = sem(0)
        barreraSalida[i] = sem(0)
        mutex[i] = sem(1)
    }

    cliente:
        int mesa = conseguirMesa()
        mutex[mesa].wait()
            cantidadJugadores[mesa] ++
            if (cantidadJugadores[mesa] == 4)
                barreraJugar[mesa].signal()
        mutex[mesa].signal()

        barreraJugar[mesa].wait()
        barreraJugar[mesa].signal()

        jugar()

        mutex[mesa].wait()
            cantidadJugadores[mesa] --
            if (cantidadJugadores[mesa] == 0)
                barreraSalida[mesa].signal()
        mutex[mesa].signal()

        barreraSalida[mesa].wait()
        barreraSalida[mesa].signal()

        abandonarMesa(mesa)

        //re-establesco semaforos a 0
        barreraSalida[mesa].wait()
        barreraJugar[mesa].wait()

## 14) El problema del barbero
Los clientes deberán hacer en orden: entrar, sentarseEnSofa, sentarseEnSilla, pagar y salir. 
Por otro lado, los barberos: cortarCabello y aceptarPago.

    //Estructuras de sincronización y variables compartidas
    mutex = sem(1)
    semSofa = sem(4)
    semSilla = sem(3)
    int gente = 0 //gente adentro < 20
    //int sofa = 0 //gente en el sofa < 4 -----CORRECCIÓN: me puedo ahorrar esta variable si aprovecho concurrencia
    //int silla = 0 //gente en las sillas < 3 -----CORRECCIÓN: me puedo ahorrar esta variable si aprovecho concurrencia
    colaParado = cola[13]      //Cola fifo
    colaSofa = cola[4]
    semPuedePagar = sem(0)
    semPuedeSalir = sem(0)

    semBarberos = sem(0)
    semAceptarPago = sem(0)

    //Código (asumo que ejecutan una sola vez)
    cliente(i):
        mutex.wait()
        if (gente == 20)
            mutex.signal()      //no entra mas gente
        else
            gente ++
            colaParado.push(i)
            mutex.signal()

            semSofa.wait()
			//espero a que desocupe el sofa. Deja pasar 4 personas al principio, el proceso i es el cliente
            mutex.wait()
                int siguienteSofa = colaParado.pop() 
                colaSofa.push(siguienteSofa)     //se sienta en el sofa   
            mutex.signal()

            semSilla.wait()     //espero a que desocupe la silla
            mutex.wait()
                int siguienteSilla = colaSofa.pop()          //se para el que mas tiempo estuvo esperando
            mutex.signal()
            semSofa.signal()
                    
            semBarberos.signal()
            semPuedePagar.wait()        //esperar a que el barbero termine
            pagar()
            semAceptarPago.signal()
            semPuedeSalir.wait()        //esperar a que el barbero haya aceptado pago
                    
            semSilla.signal()       //no me paro de la silla hasta que puedo salir, el problema no especifica cómo hacer esto.
            mutex.wait()
                gente --
            mutex.signal()
            salir()

    barbero:
        semBarberos.wait()
        cortarPelo()
        semPuedePagar.signal()
        semAceptarPago.wait()
        aceptarPago()
        semPuedeSalir.signal()

NOTA: cuando hago "int siguienteSofa = colaParado.pop()" asigno el siguiente de la lista a la variable y popea. 
Es un abuso, no estoy seguro de que exista esa operacion de igualdad.

## 15) El crucero de Noel
En el crucero de Noel queremos guardar parejas de distintas especies (no sólo un solo individuo por especie). Hay una puerta por cada especie.
Los animales forman fila en cada puerta, en dos colas, una por sexo. Queremos que entren en parejas. Programar el código de cada proceso P(i, sexo). Pista: usar dos semáforos y la función entrar(i).

| M | H | <- puertas para macho y hembra 
|:-------:|:--------:|
    //Estructura de sincronización
    puerta[n][2]
    for (int i = 0; i < n; i++) {
        puerta[i][0] = sem(0)
        puerta[i][1] = sem(0)
    }
    //-DUDA-Estoy usando N semaforos... esta bien?
    //------------------------------
    //Si estaba bien, la pista se refería a usar dos veces semáforos
    //-------------------------

    //Código
    animal(i,sexo):
        puerta[i][¬sexo].signal()
        puerta[i][sexo].wait()
        entrar(i)

## 16) La cena de los antropófagos
Una tribu de antropófagos cena usando una gran cacerola que puede contener M porciones de misionero asado. Cuando un antropófago quiere comer se sirve de la cacerola, excepto que esté vacía. Si la cacerola está vacía, el antropófago despierta al cocinero y espera hasta que éste rellene la cacerola.
Sin sincronización, el antropófago hace:

    while (true) {
        tomar_porcion();
        comer();
    }

(a) Complete el código mostrado para que los procesos tipo antropófagos, y el proceso tipo cocinero, cumplan con la idea establecida.
El código del proceso cocinero debe escribirse aparte de los antropófagos.

    //Estructuras de sincronización y variables compartidas
    mutex = sem(1)
    esperarCocinero = sem(0)
    semCocinero = sem(0)
    int porciones = M

    //Código
    antropófago:                                            
        while (true) {                                          
            mutex.wait()                                        
                if(porciones == 0)                              
                    mutex.signal()                                 
                    semCocinero.signal()                        
                    esperarCocinero.wait()                      
                else
                    porciones--
                    mutex.signal()
                    tomar_porcion();
                    comer();
        }
		
	cocinero:
		semCocinero.wait()
		rellenandoCacerola()
		mutex.wait()
			porciones = 20
		mutex. signal()
		esperarCocinero.signal()

## 17) Fiesta!
Cada heladera tiene capacidad para 15 botellas y 10 porrones. Los porrones no pueden ser ubicados en el sector de botellas y viceversa.
Las heladeras hay que llenarlas en orden. Hasta no llenar completamente una heladera (ambos tipos de envases), no pasamos a la siguiente.
Debemos enchufarlas antes de empezar a llenarlas. Una vez llena, hay que presionar el botón de enfriado rápido.
Al bar llegan los proveedores y nos entregan cervezas de distintos envases al azar, no pudiendo predecir el tipo de envase.

Dos tipos de procesos: heladera y cerveza.
Operaciones: 

- heladeras: EnchufarHeladera(), AbrirHeladera(), CerrarHeladera() y EnfriadoRapido().
- cerveza: LlegarABar() y MeMetenEnHeladera().
    
Escribir el pseudocódigo de los procesos H(i) (heladera) y C(i, tipoEnvase) (cerveza) que modelan el problema

    //Estructura de sincronización y variables compartidas
    int cantidad = 0

    mutex = sem(1)
    botellas = sem(0)
    porrones = sem(0)
    heladeras[N]
    for(int i = 0; i < N; i++) {
        if (i == 0)
            heladeras[i] = sem(1)
        else
            heladeras[i] = sem(0)
    }

    //Código
    C(i, tipoEnvase):                       // No uso el i, esta bien? SI, solo me intereza el tipo
        LlegarABar()
        if (tipoEnvase == "botella")
            botellas.wait()
        else
            porrones.wait()
			
        mutex.wait()
            MeMetenEnHeladera()            //Consigna dice meter uno a la vez
            cantidad ++
            if (cantidad % 25 == 0)
                heladeraLlena.signal()
        mutex.signal()
        

    H(i):
        heladeras[i].wait()
        EnchufarHeladera()
        AbrirHeladera()
        
        repeat 15:
            botellas.signal()
        repeat 10:
            porrones.signal()

        heladeraLlena.wait()
        CerrarHeladera()
        EnfriadoRapido()
        if (i != N-1)
            heladera[i+1].signal()

## 18) Lavarropas
Se tiene un único lavarropas que puede lavar 10 prendas y para aprovechar al máximo el jabón nunca se enciende hasta estar totalmente lleno. 
Suponer que se tiene un proceso L para simular el lavarropas y un conjunto de procesos P(i) para representar a cada prenda.
Escribir el pseudocódigo que resuelva este problema de sincronización teniendo en cuenta:

- El proceso L invoca estoyListo() para indicar que la ropa puede empezar a ser cargada.
- Un proceso P(i) invoca entroAlLavarropas() una vez que el lavarropas está listo. No pueden ingresar dos prendas al lavarropas al mismo tiempo.
- El lavarropas invoca lavar() una vez que está totalmente lleno. Al terminar el lavado invoca a puedenDescargarme().
- Cada prenda invoca saquenmeDeAquí() una vez que el lavarropas indicó que puede ser descargado y termina su proceso. Las prendas sí pueden salir todas a la vez.
- Una vez vacío, el lavarropas espera nuevas prendas mediante estoyListo().

---

    //Estructuras de sincronización y variables
    int cantidad = 0;
    prendas = sem(0)
    prendasDescarga = sem(0)
    mutex = sem(1)
    listo = sem(0)
    listoDescarga = sem(0)

    //Código
    L:
        while (true)
            estoyListo()
            repeat 10:
                prendas.signal()
            listo.wait()
            lavar()
            puedenDescargarme()
            repeat 10:
                prendasDescarga.signal()
            listoDescarga.wait()

    P(i):
        prendas.wait()
        mutex.wait()
            entroAlLavarropas()
            cantidad ++
            if (cantidad == 10)
                listo.signal()
        mutex.signal()

        prendasDescarga.wait()
        saquenmeDeAquí()
        mutex.wait()
            cantidad --
            if (cantidad == 0)
                listoDescarga.signal()
        mutex.signal()

## 19) Babuinos Cruzando
Los babuinos necesitan cruzar constantemente en ambas direcciones mediante una cuerda. 
La cuerda no es muy resistente y aguanta a un máximo de cinco babuinos simultáneamente.
Si en cualquier instante hay más de cinco babuinos en la cuerda, ésta se romperá.

(a) Asumiendo que le podemos enseñar a los babuinos a usar semáforos, diseñar un esquema de sincronización con las siguientes propiedades:

- Una vez que un babuino ha comenzado a cruzar, se garantiza que llegará al otro lado sin encontrarse con un babuino que vaya en la dirección opuesta.
- Nunca hay más de 5 babuinos en la cuerda.

---

    //Estructura de sincronización y variables compartidas.
    capacidad = sem(5)
    turno = sem(1)
    esperandoOtraDireccion = sem(1)
    mutex[2] = [sem(1), sem(1)]  //0 -> izq, 1 -> der

    int babuinosIzq = 0
    int babuinosDer = 0

    //Código
    babuino(i):
        if (i == "izquierda")
            cruzarIzquierda()
        else
            cruzarDerecha()

    void cruzarIzquierda()
        turno.wait()
            mutex[izq].wait()
                babuinosIzq ++
                if (babuinosIzq == 1)
                    esperandoOtraDireccion.wait()
            mutex[izq].signal()
        turno.signal()

        capacidad.wait()
            cruzar()
        capacidad.signal()

        mutex[izq].wait()
            babuinosIzq --
            if (babuinosIzquierda == 0)
                esperandoOtraDireccion.signal()
        mutex[izq].signal()
            
    void cruzarDerecha()
        turno.wait()
            mutex[der].wait()
                babuinosDer ++
                if (babuinosDer == 1)
                    esperandoOtraDireccion.wait()
            mutex[der].signal()
        turno.signal()

        capacidad.wait()
            cruzar()
        capacidad.signal()

        mutex[der].wait()
            babuinosDer --
            if (babuinosDer == 0)
                esperandoOtraDireccion.signal()
        mutex[der].signal()

(b) ¿La solución propuesta permite inanición?

>No, el semáforo "turno" cuando llega un babuino que quiere cruzar en la otra dirección se queda esperando el semáforo "esperandoOtraDireccion". Los babuinos que están cruzando terminan de cruzar y los que llegan en la misma dirección del que cruzó queda bloqueado ya que no pueden acceder al "turno" hasta que el del dirección  contrario salga de la zona crítica.

