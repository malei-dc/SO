# Práctica 7: Sistemas distribuidos

## 1) 4 En línea
Un sistema distribuido tiene cuatro nodos: A, B, C y D, conectados de la siguiente forma:

	A <-> B <-> C <-> D

(a) Dados los siguientes escenarios:

- El nodo B se cae
- El enlace entre A y B se cae.
- B está muy sobrecargado, y su tiempo de respuesta es 100 veces mayor a lo normal.

¿Puede A discernir entre cada uno de ellos?

>Cuando un nodo envía un mensaje puede pasar dos cosas, que reciba respuesta o sufre un timeout.
>
>No puede discernir la causa del timeout.

(b) Si A recibe un mensaje de D, a través de B, ¿Se puede asumir que D no está caído?.

>No hay garantía de que si A recibe mensaje de respuesta de D, D no esté caido. Pudo haberse caido luego de generar el mensaje y antes de que A lo reciba.

(c) Si B recibe un mensaje de A y uno de C, ¿Se puede saber si A envio su mensaje antes que C, o viceversa? ¿Por qué?

>El tiempo de llegada real es imposible saberlo ya que existen muchos factores que nos dan información erronea:
>
>- Delay en mensajes
>- Relojes desincronizados
>- Timestamp viejos
>
>Pero se puede definir un orden basandonos en timestamp u otros métodos

## 2) n procesadores
Suponer un sistema donde los n procesadores están comunicados mediante un bus ultra rápido de baja latencia, de manera tal que los tiempos de acceso a memoria remota son comparables con los tiempos locales. Imaginar que se cuenta además con un entorno de programación que permite manejar la memoria remota como si fuera local. ¿Consideraría a tal sistema como distribuido o paralelo? Justificar.

>Se consideraría como distribuidos ya que habla sobre acceso a memoria remota, nos da la noción de que cada procesador tiene un clock diferente.
>
>La noción general sería: 
>- Multiples clocks -> sistema distribuido
>- Un solo clock -> sistema paralelo

## 3) Algoritmo de commit
Un algoritmo de commit distribuido funciona de la siguiente manera: opera sobre una red donde los paquetes pueden perderse o demorarse, y cuando un nodo quiere escribir sobre cierto archivo que está replicado en todos los nodos, envía un pedido de escritura. Si recibe confirmación de todos los nodos, escribe y le notifica a los demás que pueden hacer lo propio.
Alguien nota que este algoritmo puede fallar si los nodos se caen entre la escritura y la notificación, y propone para solucionarlo el envío de mensajes de confirmación de parte de los nodos. ¿Este algoritmo modificado resuelve el problema? Justificar.

>Puede fallar si se caen los nodos antes de enviar su segunda confirmación. También puede fallar si se cae el nodo que escribió antes de enviar la notificación de que ya escribió.

## 4) Topología anillo
Se tiene un sistema distribuido donde los nodos están conectados bajo una topología de anillo, cada uno con un ID que los identifica. Se quiere implementar un algoritmo de elección de líder donde aquel elegido sea el que tenga el menor ID.

(a) Proponga un protocolo para resolver este problema. Analize la complejidad de su protocolo con respecto a la cantidad de mensajes que se utilizan.

>El proceso de selección de líder sucede de la siguiente forma, desde la óptima de un nodo cualquiera. (Algoritmo LCR)
>
>Se apunta él mismo como líder y le envía a su nodo sucesor en el anillo un mensaje para dar inicio a la selección de líder, en donde incluye su propio PID.
>
>Cuando un nodo recibe un mensaje de su predecesor, compara el PID recibido con el suyo y toma una acción:
>- Si el PID recibido es menor que el propio, lo guarda como nuevo líder y reenvía ese PID a su sucesor.
>- Si el PID recibido es mayor que el propio, mantiene el líder que ya tenía y envía ese PID a su sucesor.
>- Si el PID recibido es el suyo, entonces no envía ningún otro mensaje y se mantiene como líder.

>Si un solo nodo inicia la selección, entonces se envían O(2n) = O(n) mensajes. El mejor caso es cuando el nodo que inicia la selección es el del menor PID. El peor caso es cuando el menor PID es el predecesor del nodo que inició la selección.
>
>La selección de líder se dispara cuando un nodo detecta que el líder no responde. En el peor caso, todos los nodos detectan al mismo tiempo que el líder no responde, y todos inician el proceso de selección. Por eso el total de mensajes enviados está en el orden de n²

(b) Si su solución tuvo O(n²) cantidad de mensajes utilizados, proponga otro protocolo que disminuya esa cantidad.

>El algoritmo de Hirschberg–Sinclair es un algoritmo distribuido diseñado para el problema de elección de líder en una red de anillo sincrónico. Lleva el nombre de sus inventores, Dan Hirschberg y J. B. Sinclair.
>
>El algoritmo requiere el uso de IDs únicos (UID) para cada proceso. El algoritmo funciona en fases y envía su UID en ambas direcciones.
>
>El mensaje se envía a una distancia de $2^(Número de Fase)$ saltos y luego el mensaje regresa al proceso de origen. 
>
>Mientras los mensajes se dirigen "hacia afuera", cada proceso receptor comparará el UID entrante con el suyo propio.
>
>Si el UID es mayor que su propio UID, continuará enviando el mensaje. De lo contrario, si el UID es menor que su propio UID, no pasará la información. Al final de una fase, un proceso puede determinar si enviará mensajes en la siguiente ronda si recibió ambos de sus mensajes entrantes. Las fases continúan hasta que un proceso recibe ambos de sus mensajes salientes, de ambos vecinos. En ese momento, el proceso sabe que tiene el UID más grande en el anillo y se declara a sí mismo como el líder.

>Analizamos el algoritmo:
>
>- Fases del Algoritmo: El algoritmo trabaja en fases, donde en cada fase k, los mensajes se envían a una distancia de 2^k saltos.
>- Número de Mensajes por Fase: En cada fase, cada proceso envía dos mensajes (uno en cada dirección). Si hay nn procesos, en cada fase se envían 2n2n mensajes.
>- Número Total de Fases: El número de fases kk necesarias para que un mensaje viaje alrededor del anillo y regrese al proceso original es log(n), porque 2^k debe ser al menos n para garantizar que los mensajes han viajado todo el anillo.

>Total de Mensajes: en cada fase, se envían $2n$ mensajes. Con $log(n)$ fases, el total de mensajes sería: $2n * log(n)$.

## 5) Particiones del Sistema
Se tiene réplicas de una base de datos en m nodos distintos, cada uno con su propio identificador. Estos nodos están distribuidos en grupos en distintas partes del mundo. Todos los nodos están conectados entre sí y tienen un líder para poder organizarse. Supongamos que falla el líder.

(a) Proponga un algoritmo para elección de un nuevo líder, basándose en el menor ID.

>Propuesta: algortimo bully.
>Ventajas:
>- Cualquier Nodo puede iniciar un proceso de elección.
>- Si no se sabe cuando un lider se cae, los nodos podrían iniciar procesos de elección cada cierto tiempo.
>- Si ningún nodo se cae, las elecciones siempre dan el mismo lider.
>- Si un nodo se cae, las elecciones dan un nuevo lider.

>Idea:
>- El nodo que arranca la elección envia un mensaje "Election" a todos los sistemas con ID menor.
- Si no recibe un "OK", se declara como el nuevo lider y avisa a TODOS los sistemas con mayor ID que gano.
- Si recibe un mensaje "OK", se queda esperando por un mensaje de que otro proceso gano.
- Si un nodo recibe un mensaje de "Election", responde "OK" y arranca una elección mandando hacia IDs menores.
- Si un nodo recibe un mensaje de "I Won", considera el que lo mando como nuevo lider.

(b) Suponga que uno de los grupos queda desconectado de todos los demás, formándose una partición de la red. Después de un cierto tiempo, el grupo se puede volverse a conectar. Proponga un protocolo de elección de líder que contemple esta situación. Cada partición que se provoque tiene que elegir un nuevo líder y seguir funcionando bajo sus órdenes. En caso que se vuelvan a unir las particiones, deberán buscar un nuevo líder.

>El algoritmo Bully cumple con esta condicion
>- Si se crea una particion, eventualmente un proceso iniciara una elección y elegira un nuevo lider
>- Si se une la partición, eventualmente un proceso inciara una elección y elegir un nuevo lider

## 6) Topología en malla
Se tiene un sistema distribuido conectado con una topología en malla. Proponga algún protocolo de elección de líder bajo esta topología.

![](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Practica7/ej6malla.png)

>En este caso, como los nodos no estan interconectados todos entre sí, no se puede usar el algoritmo de bully.
>
>Dado un sistema distribuido de $n$ nodos conectados en topología de malla, podríamos utilizar el algoritmo de FloodMax para elegir un líder.
>La malla es un grafo, y este algoritmo requiere saber el diámetro del grafo: el camino más largo (en aristas) entre cualquier par de nodos.
>
>Sea n la cantidad de filas y m la cantidad de columnas en la malla. El camino más largo está dado entre nodos en 2 esquinas opuestas de la malla. El camino más largo resulta O($n + m$).
>
>Como el algoritmo es sincrónico, asumimos que tenemos algún mecanismo para coordinar cada iteración del algoritmo entre los distintos nodos. 
>En cada iteración, cada nodo envía a todos sus vecinos un mensaje con el PID más alto que conoce (inicialmente su propio PID). Podemos acotar el total de vecinos por O($4nm$) = O($nm$).
>
>El nodo con el PID más alto está a lo sumo a distancia n + m de cualquier otro nodo. Por eso repetimos este proceso n + m veces, permitiendo así al PID más alto propagarse por toda la malla. La cantidad de mensajes enviados resulta O($(n+m)(nm)$).
>
>Al finalizar, el PID más alto es el nuevo líder conocido por todos los nodos. 

## 7) Especie de Bully
Considere el siguiente algoritmo. Todos los nodos están conectados entre sí.

Paso 1: Cuando un nodo se da cuenta de que el líder se ha bloqueado, envía un mensaje “ELECCIÓN” a todos con id superior al suyo. En caso que no reciba respuestas, asume que es el líder y envía el mensaje “COORDINADOR”, junto con su id a todos los nodos.

Paso 2: Cuando un nodo con un id superior recibe un mensaje de “ELECCIÓN” de un nodo inferior, responde con su id de nodo junto con la respuesta “OK”.

Paso 3: Cuando un nodo recibe la respuesta “OK” de muchos nodos (con sus respectivos ids), encuentra el id más alto, max(id), y envía mensajes “COORDINADOR” junto con el maximo id de todos los nodos. De esta forma se anuncia al nuevo líder.

Para asegurarse de que el líder recién elegido no se haya bloqueado, el nodo que envía el mensaje “COORDINADOR” junto con el id, espera un tiempo aleatorio para recibir la respuesta “ACEPTAR” del nuevo líder. Si no recibe respuesta de “ACEPTAR”, el nodo que envía el mensaje "COORDINADOR" inicia de nuevo todos los pasos. Estos pasos continúan hasta que un nuevo líder responde con la respuesta “ACEPTAR” o el nodo sigue el Paso 1 donde anuncia al nuevo líder.

(a) Indicar si se logra elegir un líder al usar este algoritmo. Justifique su respuesta.

>Si, se logra escoger adecuadamente un lider.
>Se escoge un lider a partir de TODOS los potenciales lideres.
>Cuando se llega a un consenso, todos los nodos son avisados del nuevo lider.

(b) Indicar a cuál de los algoritmos vistos en clase se asemeja más, indicando sus ventajas y/o desventajas en comparación al algoritmo seleccionado. Justifique su respuesta.

>Se asemeja mucho al algoritmo Bully. La ventaja es que necesita menos iteraciones que el bully original, el nodo que empieza la elección cuando recibe las respuestas, compara el valor maximo del id y lo define directo. La desventaja es que al esperar a que el lider elegido "acepte" puede traer complicaciones, como que el lider se caiga antes recibir el "aceptar" o despues de haber "aceptado". 

## 8) Two Phase Commit Desentralizado
En una variante descentralizada del protocolo Two Phase Commit, los participantes se comunican directamente uno con otro en vez de indirectamente con un coordinador

- Fase 1: El coordinador manda su voto a todos los participantes
- Fase 2: Si el coordinador vota que no, los participantes abortan su transacción. Si vota que sí, cada participante manda su voto al coordinador y al resto de participantes donde cada uno decide sobre el resultado acorde a el voto que le llega y lleva a cabo el procedimiento. Es decir, si llega un solo abort, deben cancelar su ejecución. En cambio, tiene que llegar la confirmación de todos los demás para poder continuar.

(a) ¿Qué ventajas y desventajas encuentra esta variante con respecto a la variante centralizada? Hablar con respecto a la cantidad de mensajes, tolerancia a fallos, etc.

>Ventajas:
>1. Mayor Tolerancia a Fallos del Coordinador: 
>	- Descentralizada: En esta variante, si el coordinador falla después de enviar su voto, los participantes aún pueden completar la transacción comunicándose entre ellos. Esto aumenta la tolerancia a fallos del sistema.
>	- Centralizada: Si el coordinador falla, todo el proceso de commit puede quedar en un estado incierto, lo que requiere mecanismos adicionales de recuperación.
>2. Distribución de la Carga:
>	- Descentralizada: La carga de comunicación se distribuye entre todos los participantes en lugar de concentrarse en un único coordinador. Esto puede mejorar el rendimiento en redes grandes o altamente distribuidas.
>	- Centralizada: Toda la carga de comunicación recae en el coordinador, lo que puede ser un cuello de botella y afectar el rendimiento.

>Desventajas:
>1. Mayor Cantidad de Mensajes:
>	- Descentralizada: Requiere una mayor cantidad de mensajes, ya que cada participante debe enviar su voto al coordinador y a todos los demás participantes. Esto resulta en n² mensajes en total (donde n es el número de participantes), lo que puede ser ineficiente en términos de ancho de banda y tiempo de procesamiento.
>	- Centralizada: Requiere menos mensajes. En la fase 1, el coordinador envía un mensaje a cada participante (n mensajes). En la fase 2, cada participante envía su voto al coordinador y el coordinador envía la decisión final (n mensajes). En total, hay $2n$ mensajes.
>2. Complejidad de Implementación:
>	- Descentralizada: La lógica de comunicación y decisión se vuelve más compleja, ya que cada participante debe manejar la recepción y evaluación de los votos de todos los demás participantes. 
>	- Centralizada: La lógica de decisión es más sencilla, ya que solo el coordinador toma la decisión final basada en los votos de los participantes.
>3. Mayor Latencia:
>	- Descentralizada: La necesidad de esperar los votos de todos los demás participantes puede aumentar la latencia, especialmente en redes con alta latencia o en casos de participantes lentos.
>	- Centralizada: La latencia puede ser menor, ya que la comunicación se realiza en dos fases directas entre el coordinador y los participantes.

(b) ¿En qué casos usaría cada versión del protocolo?

>- Centralizada: 
>	1. Sistemas con coordinador confiable
>	2. Baja Latencia y Alta Velocidad de Comunicación: debido a la menor complejidad de comunicación.
>	3. Redes Pequeñas o de Tamaño Moderado: la menor cantidad de mensajes hacen que la variante centralizada sea más eficiente.
>- Descentralizada:
>	1. Redes Grandes y Distribuidas: mejora la tolerancia a fallos y distribuir la carga, evitando el cuello de botella en coordinador.
>	2. Alta Tolerancia a Fallos del Coordinador: permite que los participantes completen la transacción incluso si el coordinador falla.
>	3. Alta Carga de Trabajo: distribuir la carga de comunicación entre todos los participantes puede mejorar el rendimiento general.

## 9) Sistema bancario 
Un servidor usa ordenamiento por timestamps para el control de la concurrencia entre sistemas distribuidos. Este servidor es una app bancaria permita realizar operaciones de manera remota. Entre otras cosas, comprar y vender diferentes tipos de divisas. Un usuario puede comprar cualquier tipo de moneda dentro del sistema con cualquiera que posea en su cuenta. El banco cuenta con un sistema que recibe los pedidos, que a su vez se puede comunicar con otro sistema que controla el stock de los diferentes tipos de divisas y un tercer sistema que mantiene registro del estado de cuenta de los usuarios.

(a) Describir un protocolo que permita a los usuarios comprar divisas manteniendo la consistencia, aprovechando el uso de los timestamps.

>Mantenimiento de Consistencia
>- Timestamps: Se usan para asegurar el orden de las transacciones. Cada transacción lleva un timestamp T que ayuda a los participantes a decidir si deben proceder con la transacción en base a la temporalidad.
>- Logs de Transacciones: Cada sistema debe mantener un log de las transacciones con los timestamps para recuperarse de fallos y para asegurar la consistencia en caso de que el sistema se reinicie.
>
>Componentes del Sistema
>1. Cliente (Usuario): Inicia las solicitudes de compra o venta de divisas.
>2. Servidor de Pedidos: Recibe las solicitudes de los usuarios y coordina la transacción.
>3. Sistema de Control de Stock de Divisas: Mantiene el registro del stock de las diferentes divisas.
>4. Sistema de Estado de Cuenta de Usuarios: Mantiene el registro del saldo de los usuarios en diferentes divisas.

>Protocolo (3PC)
>Fase 1: Preparar
>- Cliente: Envía solicitud con timestamp T.
>- Servidor de Pedidos: Envía "Prepare" a los participantes con timestamp T.
>- Participantes: Verifican condiciones y responden con "Prepared" o "Abort" y timestamp T. (verifican en ambos sistemas el estado)
>
>Fase 2: Precommit
>- Servidor de Pedidos: Recibe votos, decide y envía "Precommit" o "Abort" a los participantes.
>- Participantes: Preparan commit o abortan.
>
>Fase 3: Commit
>- Servidor de Pedidos: Envía "Commit" o "Abort" basado en confirmaciones.
>- Participantes: Completan la transacción o la abortan. (y actualizan en ambos sistemas nuevo estado)

## 10) Paxos - No vimos paxos.