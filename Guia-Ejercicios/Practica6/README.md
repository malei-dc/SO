# Práctica 6: Sistemas de archivos

## 1) Accesos FAT
Suponer una computadora cuyo disco se accede sin memoria cache y un sistema de archivos FAT. Además, en este sistema, la FAT no queda almacenada en la memoria (recordar que lo normal es que la FAT se cargue en memoria). ¿Cuántos accesos al disco son necesarios para llegar hasta el último bloque de un archivo de N bloques?

>Nuestro objetivo es acceder a todos y cada uno de los bloques
>Suponemos que conocemos el bloque por el cual empezar
>
>Vamos a tener que seguir los siguientes pasos
>1. Leer el primer bloque de memoria
>2. Consultar la FAT por el siguiente
>3. Leer el siguiente bloque de memoria
>4. Repetir pasos II y III otras N - 2 veces
>
>Los pasos I, II y III requieren un acceso al disco
>El paso IV requiere repetir N - 2 veces 2 accesos al disco, dando un total de 2N - 4 accesos a disco
>Sumando todos los accesos al disco tenemos un total de 2N - 1  accesos al disco

## 2) Sistema FAT
Se tiene un disco con capacidad de 128 GB, y bloques de 8 KB. Supongamos un sistema de archivos similar a FAT, donde la tabla se ubica desde la posición 0, y cada entrada de la tabla es de 24 B

(a) ¿Qué tamaño ocupa la tabla?

>1° calculamos cantidad de bloques: 
1GB = 1048576KB -> 128GB = 134217728KB eso lo dividimos por tamaño de los bloques y obtenemos cantidad de bloques.
cantidad de bloques = 134217728KB / 8KB = 16777216 bloques
>2° el tamaño de la tabla es la cantidad de bloques por el tamaño de las direcciones en este caso 24B tamaño de la tabla = 16777216 * 24B = 402653184B -> 384MB

>La tabla ocupa 384MB de disco

(b) ¿Cuántos archivos de 10 MB es posible almacenar?

>Primero hay que ver cuántos bloques son 10MB. 1MB = 1024KB -> 10MB = 10240KB
>Cantidad de bloque que ocupa el archivo = 10240KB / 8KB = 1280 bloques
>Como tenemos 16777216 bloques pueden entrar 13107 archivos de 10MB
    
(c) Se sabe que un archivo comienza en el bloque 20. Dada la siguiente FAT, indicar el tamaño de dicho archivo.

| Bloque | 0 | 1 | 2 | 3 | 4 | 5 | 6 | ... | 20 | 21 | 22 | ... |
|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|
| Siguiente | EOF | 2 | 23 | 4 | 5 | 0 | 7 | ... | 21 | 22 | 3 | ... |

>El archivo ocupa los bloques: 20 21 22 3 4 5 0
>En total son 7 bloques, entonces el archivo tiene tamaño aproximado entre 48~56KB ya que el último bloque puede no estar usada en su totalidad.

## 3) UNIX maso ext2
Un sistema de archivos de UNIX, similar a ext2, tiene bloques de tamaño 4 KB y el direccionamiento a bloques de disco (LBA) es de 8 B. 
A su vez, cada inodo cuenta con 5 entradas directas, dos indirectas y una doblemente indirecta. 

(a) ¿Cuál es el tamaño máximo de archivo que soporta?

>Para saber esto necesito saber la cantidad de bloques y el tamaño de cada bloque. Esto segundo nos lo da el enunciado, tamaño de bloque de 4KB.
>Antes de calcular la cantidad de bloques, calculemos primero la canrtidad de inodos que entran en un bloque:
>Bloque de 4KB, direcciones de 8B. 4KB = 4096B -> 4096B / 8B = 512 bloques.
>Calculamos la cantidad de bloques:
>- 5 directas = 5 bloques
>- 2 indirectas, cada indirecta apunta a un bloque de 4KB que contiene direcciones de 8B, como calculamos arriba, son 512 bloques por indirecta, como son dos, en total son 1024 bloques.
>- 1 doble indirecta, apunta a un bloque que tiene 512 indirectas, 512 * 512 bloques = 262144 bloques 

>En total tenemos 5 + 1024 + 262144 = 263173 bloques de 4KB, concluyendo con 1052692 KB ~ 1GB tamaño máximo que soporta.

(b) Si el 50 % del espacio en disco está ocupado por archivos de 2 KB, el 25 % por archivos de 4 KB y el 25 % restante por archivos de 8 KB, ¿qué porcentaje del espacio en disco está siendo desperdiciado? (Considerar sólo el espacio utilizado en los bloques de datos).

>50% = 263173 bloques / 2 = 131586 bloques . 2KB desperdiciado = 163173 KB desperdiciado (bloques son tamaño 4KB)
>25% de archivos 4KB = 0KB desperdiciado (1 bloque por archivo)
>25% de archivos 8KB = 0KB desperdiciado (2 bloques por archivo)

>En total se desperdicia aprox un 25%.

(c) ¿Cuántos bloques es necesario acceder para procesar completo un archivo de 5 MB?

>1MB = 1024KB -> 5MB = 5120KB
>5120KB / 4KB (tamaño de bloque) = 1280 bloques (si el archivo empieza desde el bloque 0, se llega a acceder hasta el primer indirecto)

## 4) ext2 - inodos
Suponer que se cuenta con un sistema de archivos basado en inodos como ext2 y bloques de 4 KB.

(a) Si se tiene un archivo de 40 KB, ¿cuántos bloques es necesario leer para procesarlo completamente?

>cantBloques = tamaño de archivo / tamaño bloque = 40KB / 4KB = 10 bloques asumiendo que comienza desde el bloque directo 0

(b) ¿Cuántos bloques es necesario leer si el archivo tiene 80 KB?

>Siguiendo la lógica del ejercicio anterior necesitaría 20 bloques. Pero si asumimos que comienza desde el bloque directo #0, leemos todas las directas y tendremos que acceder al la indireccion simple. Entonces queda 12 bloques directos, 1 indireccion simple, 8 bloques de datos restantes, en total 21.

## 5) Bloques en ext2
Considerando un sistema ext2 (12 entradas directas, 3 indirectas, 1 doble indirecta y 1 triple indirecta) y un sistema basado en FAT:
¿Cuántos bloques de disco se deben acceder para leer los siguientes bloques de un archivo?

(a) 1, 2, 3, 5, 7, 11, 13, 17, 23.

>- FAT: accedemos a los bloques, no es necesario acceder cual es el siguiente. #bloques accedidos = 9
>- ext2: accedemos a los bloques especificos. #bloques accedidos = 10 (9 bloque de datos + 1 indirección simple)

(b) 1, 2, 3, 4, 5, 6, 10001.

>- FAT: #bloques accedidos = 7
>- ext2: #bloques accedidos = 9 (7 bloques de datos + 2 de la indireccio doble)

(c) 13, 10000, 1000000.
    
>- FAT: #bloques accedidos = 3
>- ext2: #bloques accedidos = 8 (3 bloques de datos + 2 indirección doble + 3 indirección triple)

## 6) Directorio - FAT
Considerar la siguiente ruta a un archivo: /home/aprobar.txt.
En un sistema basado en FAT, ¿cuántos bloques de disco se deben acceder para llegar a los bloques de datos de este archivo?

>Revisemos el recorrido
>- FAT: Indispensable para conocer donde estan los bloques siguientes
>- Todos los Bloques del directorio raiz:
>	- Indica el bloque inicial de Home
>	- Pueden ser varios y los conseguiremos a traves de la FAT
>- Todos los bloques del directorio Home
>	- Indica el bloque inicial del archivo
>	- Pueden ser varios y los conseguiremos a traves de la FAT
>- Todos los bloques de aprobar.txt

>Tuvimos que acceder a 4 bloques como minimo: 1 FAT + 1 Directorio Raiz + 1 Directorio Home + 1 Aprobar.txt
>Tal vez debamos acceder a más si alguno ocupa más de 1 bloque 

## 7) ext2 o FAT
Una compañía que fabrica discos rígidos decide emprender la creación de un nuevo filesystem. Discuten la conveniencia de adoptar un enfoque inspirado en FAT o la de uno basado en inodos. Indicar cuál de las dos opciones recomendaría, y por qué, para cada uno de los siguientes requerimientos:

(a) Es importante que puedan crearse enlaces simbólicos.

>Enlaces simbolicos: Enlace que permite a 2 archivos compartir el mismo bloque de datos
>Estrategia escogida: Inodos
>
> Los inodos nos permiten definir Hard y Soft Links
> Estos enlaces permiten a 2 archivos distintos utilizar el mismo inodo.
> FAT no soporta enlaces simbolicos.

(b) Es importante que la cantidad de sectores utilizados para guardar estructuras auxiliares sea acotada, independientemente del tamaño del disco.

>Estrategia escogida: Inodos
>
> FAT tiene un tamaño proporcional a la cantidad de bloques y al tamaño de disco en consecuencia.
> Mayor disco implica mayor tamaño de la FAT.
> La FAT es una estructura auxiliar.
> Eso significa que la cantidad de sectores utilizados para guardar la FAT no está acotada.
>
>Los inodos en cambio tienen un tamaño acotado.
>Cada inodo ocupa un determinado tamaño y solo hay un inodo por archivo.

(c) Es importante que el tamaño máximo de archivo sólo esté limitado por el tamaño del disco.

>Estrategia escogida: FAT 
>
>En FAT un archivo puede ocupar todo el disco si es necesario
>Todos los bloques de la tabla de FAT perteneceran a este archivo
>
> En Inodos un archivo puede ocupar tantos bloques como tenga el inodo
> La cantidad de bloques del inodo puede ser menor a la cantidad de bloques disponibles de disco (y suele serlo)
> Eso significa que el tamaño maximo de una archivo va a estar limitado por la cantidad de bloques que referencie el inodo

(d) Es importante que la cantidad de memoria principal ocupada por estructuras del filesystem en un instante dado sea (a lo sumo) lineal en la cantidad de archivos abiertos en ese momento.

>Estrategia escogida: Inodos

>Un archivo abierto solo necesita información del inodo que le corresponde
>No es necesario tener en memoria, inodos de archivos que no estamos usando

>La FAT en cambio tiene información de todos los bloques
>Entonces tenemos información en memoria de otros archivos además de los que están abiertos

>La cantidad de memoria ocupada por la FAT es la misma para cualquier cantidad de archivos abiertos
>La cantidad de memoria ocupada por los Inodos es lineal con respecto a la cantidad de archivos abiertos 

## 8) HashFS (FAT)
Se tiene un disco rígido de 16 GB de espacio con sectores de 1 KB. Se desea dar formato al disco usando un sistema de archivos de uso específico llamado HashFS, basado en FAT. La idea es que no existen directorios ni archivos. Dado un path, se calcula el hash del nombre y éste indica cuál es el archivo buscado. Este sistema de archivo cuentas con dos tablas:

- Una única FAT que guarda las entradas correspondientes al próximo bloque, indicando el final de un archivo cuando estos valores coinciden.
- Una única tabla de hash que contiene, para cada hash posible, el identificador del bloque inicial y el tamaño en bytes del archivo correspondiente a dicho hash.

Permite configurar los siguientes elementos:

- Tamaño del bloque: 2, 4 u 8 sectores.
- Tamaño de identificadores de bloque: 8, 16, 24 o 32 bits.
- Tamaño del hash: 8, 16, 24 o 32 bits.

(a) Suponiendo que se configura con 2 sectores por bloque, identificadores de bloque de 24 bits, y hash de 16 bits. 

>Traducido: 2KB por bloque, direcciones de 24 bits

¿Cuál es el tamaño que ocupa la FAT? 

>El tamaño de la tabla de FAT se calcula como cantBloques x tamaño de direcciones 
>cantBloques = 16GB (memoria total) / 2KB (tamaño del bloque) = (16 x 1024 x 1024)KB / 2KB = 8.388.608 bloques 
>TamañoFAT = 8.388.608 x 24bits = 201.326.592 bits = 25.165.824B = 24.576KB = 24 MB

¿Cuál es el tamaño de la tabla de archivos? 

>El tamaño de la tabla de archivos se calcula con #bloques * tamaño del hash
>TamañoTablaArch = 8.388.608 x 16 bits = 134.217.728 bits = 16.777.216B = 16.384KB = 16 MB

¿Cuál es el espacio que queda en disco para archivos?

>Le quedan 16GB - 40MB = 15GB 984MB

(b) Sabiendo que se desea maximizar la cantidad de archivos que el sistema soporta y que, además, en promedio los archivos tendrán un tamaño de 1 KB, ¿cuál sería la configuración óptima del sistema de archivos? Justificar.
            
>(2 Sectores por bloque)
Mas sectores por Bloque produce bloques que ocupan mas disco.
>Si elegimos 4 u 8 sectores por bloque.
>Cada bloque ocuparia 4KB y 8KB respetivamente.
>Si cada archivo ocupa un 1KB, entonces estariamos desperdiciando mucha memoria por bloque.
>
>(Identificadores de 24 bits) 
>Tenemos 2 sectores por bloque.
>Por el ejercicio anterior eso nos da un total de 8.388.608 Bloques.
>Necesitamos representar cada uno de ellos con un numero diferente.
>
>- Con 24 bits podemos representar alrededor de 16 millones de numeros diferentes.
>- Con 16 bits nos quedamos cortos.
>
>(Tamaño del Hash: 32 bits) 
>El tamaño promedio del archivo es 1024 Bytes.
>Necesitamos alrededor de 6 bits para representar 1024 numeros diferentes y 24 bits para el identificador (30 en total).

c) ¿Cómo lo configuraría si el promedio de tamaño de archivos es de 16 MB?

>Cada archivo tiene un tamaño promedio de 16.384 KB
>
>(8 sectores por bloque)
Cada bloque ocuparía 8 KB.
Si cada archivo ocupa 16.384 KB, necesitamos alrededor 2048 bloques para guardarlos.
No existe tamaño mayor a 8 sectores por bloque.
Con tal de minimizar el uso de bloques por archivo, elegimos 8 sectores por bloque.
>
>(Identificadores de 24 bits)
>Cada bloque ocupa 8 KB 
>Tenemos 16.777.216 KB de disco
>Tendremos 16.777.216 / 8 Bloques = 209652 Bloques
>Todavía no nos alcanza con 16 bits para representar está cantidad de bloques
>Nos tenemos que quedar con 24 bits. 
>
>(Tamaño del Hash: 32 bits)
>Es claro que vamos a necesitar incluso más bits que el ejercicio anterior para representarlo.
>Incluso si no nos alcanza 32 bits para representar el Hash, debemos escogerlo como configuración ideal.
>A mayor cantidad de bits, menor numero de colisiones en los Hash.

## 9) Nombre de un directorio
Linux permite pasar un descriptor de archivo de un proceso a otro (es decir, el valor de retorno de la syscall open()). Asuma que un proceso abre el directorio /home que está sobre una partición de Ext2. El proceso envía el descriptor del archivo home a otro proceso con los permisos necesarios para poder leerlo.
Asumiendo que tiene la función auxiliar 

	Ext2FSInode load_inode(int inode_number): Dado un número de inodo devuelve la estructura del inodo
        
Escriba el pseudocódigo que le permita obtener el nombre del directorio (home) a partir del descriptor de archivo recibido.

>Obtener el nombre del directorio a partir del file descriptor requiere transcionar a traves de varias tabla
>Hay que recordar la tabla que se muestra en la clase de IPC

>Cada proceso tiene su propia tabla de file descriptors (Process Table)
>Cada file descriptor referencia a una tabla propia (Open File Table)
>Esa tabla tiene un puntero a una tabla de Vnode (Vnode Table)
>Esa tabla de Vnode tiene el numero de inodo

## 10) FAT - función cargar_archivos
Se tiene un disco formateado con FAT para el cual se quiere poder devolver el contenido de un archivo a partir de su ruta absoluta. 
Para ello se debe implementar la función:

	datos = cargar_archivo(directorios[])

donde directorios es la lista de los nombres de los directorios de la ruta (ordenados, incluyendo el nombre del archivo, y sin incluir al directorio raíz). Es decir, si el archivo a abrir es \Documentos\Users\foto.png entonces directorios = [’Documentos’, ’Users’, ’foto.png’].

Para ello se cuenta con las siguientes funciones auxiliares:

- *FAT_entry*(block_address): devuelve la entrada de la tabla FAT de la posición block_address.
- raw_data = *read_blocks*(block_address1,block_address2,...): lee del disco todos los bloques indicados por parámetro, en orden.
- *parse_directory_entries*(raw_data): devuelve una lista de struct_entrada_directorio, donde cada elemento representa los subdirectorios del directorio pasado en raw_data.
- *raw_data = root_table*(): devuelve los datos de la tabla de directorios de root.
    
(a) Enumerar tres campos que debe tener la estructura struct_entrada_directorio según este tipo de filesystem.

>- Name: nombre del archivo, necesario para matchear con los nombres de la variable directorio.
>- Size: tamaño en bloques del archivo, necesario para saber cuando parar en la FAT.
>- Block_Adress: adress del bloque inicial, necesario para sacar los block_adress restantes de la FAT.

(b) Escribir el pseudo-código de la función cargar_archivo.

        struct entrada_directorio {
            char[] name
            int size
            int block_adress
        }

        raw_data cargar_archivo(char directorios[]) {
            raw_data = root_table()
            for (int i=0; i < sizeof(directorios); i++) {
                dirEntries = parse_directory_entries(raw_data);
                for (int i=0; i < sizeof(dirEntries); i++) {
                    if (dirEntry -> name == directory) {
                        block_address_list[dirEntry -> size]
                        block_address_list[0] = dirEntry -> block_adress
                        adress_for_fat = dirEntry -> block_adress
                        for (int i=0; i < dirEntry -> size) {
                            adress_for_fat = FAT_entry(address_for_fat)
                            block_adress_list[i + 1] = adress_for_fat
                        }
                        raw_data = read_blocks(block_adress_list)
                    }
                }
            }
        }
