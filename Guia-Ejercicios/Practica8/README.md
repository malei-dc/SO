# Práctica 8: Protección y seguridad

## 1) Contraseñas almacenadas.
En muchos sistemas las contraseñas de los usuarios no se almacenan, sino que se almacena solo el resultado de una función de hash. Suponiendo que la función de hash utilizada entrega valores de 64 bits y que los resultados se encuentran bien distribuidos, responder:

(a) ¿Cómo puede verificar el sistema si un usuario ingresó su contraseña correctamente, si no la tiene almacenada en el sistema?

>El sistema almacena el resultado de una función de Hash.
>Cuando un usuario ingresa su contraseña, ejecuta la función de Hash con la contraseña ingresada.
>Si el resultado es identico al tiene almacenado entonces puede decir que el usuario ingreso correctamente su contraseña.

(b) Si bien existen varias contraseñas que entregan el mismo valor de hash (colisión), ¿qué probabilidad hay de acertar el valor de hash almacenado?

>Según la consigna, la función de hash utilizada entrega valores de 64 bits y que los resultados se encuentran bien distribuidos. La cantidad de hash posibles son 2⁶⁴, entonces la probabilidad de acertar el valor de hash almacenado es 1/2⁶⁴ = 5,4210 * 10^-20. Es una posibilidad sumamente pequeña.
 
 (c) ¿En cuántos años se puede tener un 50 % de probabilidad de haber acertado al valor de hash almacenado, dado que pueden probarse mil millones de contraseñas por segundo?

>Calculemos la probabilidad de atinarle a una contraseña cualquiera:
>- Un segundo: (10 mil millones x  5,4210 x 10^-20) = 5,4210 x 10^-10 % 
>- Un minuto: 60 x 5,4210 x 10^-10 = 3,2526 x 10^-8 %
>- Una hora:  60 x 3,2526 x 10^-8  = 1,9515 x 10^-6 %
>- Un dia: 24 x 1,9515 x 10^-6 = 4,6837 x 10^-5 %
>- Un año: 365 x 4,6837 x 10^-5 = 0,01709%
>- 1000 años: 1000 x 0,01709 = 17,09%
>- 3000 años: 3000 x 0,01709 = 51,28%

(d) Conocer el valor de hash de una contraseña no implica conocer una contraseña que genere ese valor. Suponer que se pueden probar contraseñas a la misma velocidad que en el ítem anterior, pero nos informan que la contraseña es de, a lo sumo, 6 caracteres, y que estos solo son letras minúsculas o dígitos. ¿Cuánto tiempo hay que esperar para averiguar la clave?

>- Hay 26 letras minusculas en el alfabeto
>- Hay 10 digitos numericos
>- Contraseñas de a lo sumo 6 caracteres
>
>La cantidad de contraseñas es igual a la cantidad de subconjuntos de 6 caracteres que podamos armar con 36 caracteres (permitiendo repetidos).
>Vamos a tener un número combinatorio $\binom{36 + 6 - 1}{6}$ de contraseñas.
>Vamos a tener 539.566.560 contraseñas
>
>Teniendo en cuenta que podemos tirar unas 10 mil millones de contraseñas por segundo.
>Vamos a tardar menos de 1 segundo en averiguar la contraseña

## 2) Sistema de autenticación remota
Los sistemas de autenticación remota, donde el usuario se debe autenticar a través de un canal inseguro (que puede ser visto por terceros), como por ejemplo POP3, a menudo no transmiten la contraseña en el proceso de autenticación para evitar que ésta sea interceptada.

(a) ¿Sería seguro autenticar al usuario enviando por el canal (desde el usuario hasta el sistema) el hash de la contraseña? 

>No, porque un atacante puede interceptar el hash y luego utilizarlo para autenticarse.

¿A qué ataque es vulnerable este esquema?

>Este metodo es especialmente vulnerable a replay-attacks.

(b) Un esquema Challenge-Response basado en una función de hash opera de la siguiente manera:

- El sistema envía una cadena seed elegida al azar.
- El usuario responde con el resultado de hash(seed + contraseña).
- El sistema hace la misma cuenta de su lado y verifica el resultado.

Si un atacante captura toda esta conversación, ¿sería capaz de realizar un ataque de fuerza bruta sobre la contraseña sin realizar ninguna interacción con el servidor (ataque offline)?

>Sería mucho mejor que la opción anterior. Ya no puede interceptar el hash y luego reusarlo en otro intento de autenticación porque al cambiar el seed se generaría un hash distinto.
>
>No obstante, si el atacante obtiene el seed y también el hash generado por el usuario, puede intentar mediante un ataque de fuerza bruta offline encontrar una contraseña que sumada al seed genere el mismo hash interceptado. Hay altas chances que la contraseña encontrada sea la correcta, pero como las funciones de hash pueden tener colisiones, existe la posibilidad que la contraseña encontrada no sea la correcta.
>
>La seguridad de este mecanismo viene más por la complejidad en encontrar la imagen del hash, con algoritmos de hashing modernos el tiempo requerido es tan grande que un ataque de fuerza bruta es simplemente inviable.

## 3) Buffer stack overflow
Si el usuario tiene control sobre la entrada estándar, el siguiente código tiene problemas de seguridad.

    void saludo(void) {
        char nombre[80];
        printf("Ingrese su nombre: ");
        gets(nombre);
        printf("Hola %s!\n", nombre);
    }

Se considera como un problema de seguridad que un usuario atacante pueda realizar operaciones que no le eran permitidas, ya sea tanto acceder a datos privados, ejecutar código propio en otro programa o inhabilitar un servicio. Determinar:

(a) ¿Dónde se introduce el problema de seguridad?

>En el llamado a gets(nombre), ya que la función gets lee de stdin hasta encontrar un newline o EOF. Podemos ingresar un string arbitrariamente largo que supere el tamaño del buffer de 80 caracteres.

(b) ¿Qué datos del programa (registros, variables, direcciones de memoria) pueden ser controladas por el usuario?

>Si hacemos que el programa lea más de 80 caracteres, podemos generar un buffer overflow y pisar variables locales y la dirección de retorno de saludo que se encuentran en el stack.

(c) ¿Es posible sobreescribir la dirección de retorno a la que vuelve la llamada de alguna de las funciones printf o gets?

>No. Porque al llamar a estas funciones se configura el stackframe según la convención C, pisando cualquier valor que podamos haber escrito con el buffer overflow. Pero permiten alterar la dirección de retorno de la función que las invoca. Esto significa que cambiara la dirección de retorno de la función saludo.

(d) ¿Se soluciona el problema de seguridad si se elimina el segundo printf?

>No, el problema persiste en el gets donde podemos pisar la dirección de retorno de saludo.

## 4) Buffer stack overflow
El siguiente es un sistema de login que valida los datos contra una base de datos.

    struct credential {
        char name[32];
        char pass[32];
    }
    bool login(void) {
        char realpass[32];
        struct credential user;
            // Pregunta el usuario
        printf("User: ");
        fgets(user.name, sizeof(user), stdin);
            // Obtiene la contraseña real desde la base de datos y lo guarda en realpass
        db_get_pass_for_user(user.name, realpass, sizeof(realpass));
            // Pregunta la contraseña
        printf("Pass: ");
        fgets(user.pass, sizeof(user), stdin);
        return strncmp(user.pass, realpass, sizeof(realpass)-1) == 0;
            // True si user.pass == realpass
    }

Suponiendo que la función db_get_pass_for_user() es totalmente correcta y no escribe fuera de realpass():

(a) Hacer un diagrama de cómo quedan ubicados los datos en la pila, indicando claramente en qué sentido crece la pila y las direcciones de memoria. Explicar, sobre este diagrama, de qué datos (posiciones de memoria, buffers, etc.) tiene control el usuario a través de la función fgets().

        |------------------------| 0xFFFF
        |                        |
        | ..direcciones altas..  |
        |                        |
        | return address         |
        | saved rbp              |
        | realpass[24..31]       | controlado por el usuario hasta acá
        | realpass[16..23]       |
        | realpass[8..15]        |
        | realpass[0..7]         |
        | user.pass[24..31]      |
        | user.pass[16..23]      |
        | user.pass[8..15]       |
        | user.pass[0..7]        |
        | user.name[24..31]      |
        | user.name[16..23]      |
        | user.name[8..15]       |
        | user.name[0..7]        | controlado por el usuario desde acá
        |                        |
        | ..direcciones bajas..  |
        |                        |
        |------------------------| 0x0000 (crece hacia arriba)

>fgets limita la cantidad de caracteres a leer
>
>Erroneamente los fgets del programa usan el tamaño del struct como limite de caracteres para sus parametros
>- El tamaño del struct es de 64 bits
>- El tamaño de cada parametro del struct es de 32 bits
>
>El usuario pueda controlar los 32 bits contiguos a la direcciones de memoria asignadas a los parametros del struct

(b) Indicar un valor de la entrada, que pueda colocar el usuario, para loguearse como admin sin conocer la contraseña de este.

>usuario: elquecorresponda
>pass: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa 

## 5) Mas buffer overflow
La siguiente función se utiliza para verificar la contraseña del usuario actual user en el programa que desbloquea la sesión luego de activarse el protector de pantalla del sistema. El usuario ingresa la misma por teclado.

    bool check_pass(const char* user) {
        char pass[128], realpass[128], salt[2];
            // Carga la contraseña (encriptada) desde /etc/shadow
        load_pass_from(realpass, sizeof(realpass), salt, user, "/etc/shadow");
            // Pregunta al usuario por la contraseña.
        printf("Password: ");
        gets(pass);
            // Demora de un segundo para evitar abuso de fuerza bruta
        sleep(1);
            // Encripta la contraseña y compara con la almacenada
        return strcmp(crypt(pass, salt), realpass) == 0;
    }

- void *load_pass_from*(buf, buf_size, salt, user, file) : lee del archivo file la contraseña encriptada del usuario user y la almacena en el buffer buf escribiendo a lo sumo buf_size-1 caracteres y un "\0" al final de estos; guarda además en salt el valor de los dos caracteres que se usaron para encriptar la contraseña guardada en file. Si el usuario no se encuentra, se almacena en buf un valor de contraseña inválido, no generable por la función crypt.
- char *crypt(pass, salt): devuelve la contraseña pass encriptada usando el valor salt.
- /etc/shadow: almacena información sensible del sistema y un usuario común no tiene acceso a este archivo, tiene permisos r-- --- --- root root /etc/shadow.

(a) La línea 4 del código hace un llamado a función que debe leer el archivo /etc/shadow, protegido para los usuarios sin privilegio. Explicar qué mecanismo permite lanzar este programa a un usuario normal y hace que éste tenga acceso a /etc/shadow sin alterar los permisos del archivo.

>El mecanismo es principalmente el bit setuid configurado en el binario del programa. Este bit le indica al SO que ejecute el binario como el usuario owner del mismo, independientemente del usuario que invocó al binario. No obstante, sí es necesario que el usuario invocador tenga permiso para hacerlo.
>
>En síntesis, el binario debe tener el bit setuid, debe tener como owner root, y el usuario debe tener permiso para ejecutar el binario (ya sea porque pertenece al grupo del binario y éste tiene permiso de ejecución, o directamente porque el binario tiene permiso de ejecución para others, o sea, para cualquier usuario).

(b) Explicar por qué esta función tiene problemas de seguridad. ¿Qué datos sensibles del programa controla el usuario?

>Hay 2 datos sensibles:
>- user: El usuario podria alterar maliciosamente la variable de entorno del usuario
>- pass: Podria sobreescribir el valor de real-pass por no estar sanitizada
>
>Misma situación que el ejercicio 4, en el llamado a gets el usuario puede pisar las variables locales realpass y salt.

(c) Si solo los usuarios que ya ingresaron al sistema, que de por sí pueden ejecutar cualquier programa desde la terminal, son los que pueden utilizar el protector de pantalla y por tanto esta función, ¿este problema de seguridad en el protector de pantalla del sistema compromete la integridad del sistema completo? Justificar.

>Sí, porque un atacante puede tener acceso físico a la computadora.
>Usuarios maliciosos que puedan correr está función también pueden correr cualquier programa, es innecesario explotar esta vulnerabilidad para atacar el sistema. Usuarios inocentes podrían caer en el problema del pass (al escribir una pass excesivamente grande), rompiendo la ejecución sin tener intenciones de ello.

## 6) Nan floats
Considerando que el usuario controla el valor del parámetro f, analizar el siguiente código de la función signo.

    #define NEGATIVO 1
    #define CERO 2
    #define POSITIVO 3

    int signo(float f) {
        if (f < 0.0) return NEGATIVO;
        if (f == 0.0) return CERO;
        if (f > 0.0) return POSITIVO;
        assert(false && "Por aca no paso nunca");
        return 0; // Si no pongo esto el compilador se queja =(
    }

(a) ¿El usuario tiene alguna forma de que se ejecute el assert()? Pista: Piense en el formato IEEE-754.

>Si, si el usuario pasara un NaN (not a number) estos son usados para representar resultados de operaciones invalidas cuyo resultados son desconocidos (Ejemplo: 0/0). CUALQUIER COMPARACION entre un numero NAN y otro es FALSA. 

(b) En las versiones “release” los assert suelen ignorarse (opción de compilación). ¿Sería seguro utilizar la función signo sobre un dato del usuario y esperar como resultado alguno de los valores 1, 2 o 3?

>No seria seguro, como vimos anteriormente, podría no ejecutar ninguno de los 3 casos

## 7) Stack Randomization
Un esquema de protección implementado por algunos sistemas operativos consiste en colocar el stack del proceso en una posición de memoria al azar al iniciar (stack randomization). Indique cuáles de las siguientes estrategias de ataque a una función con problemas de seguridad se ven fuertemente
afectadas por esta protección, y explique por qué:

(a) Escribir el valor de retorno de una función utilizando un buffer overflow sobre un buffer en stack dentro de dicha función.

>Stack Randomization NO afecta este ataque.
>El valor que buscamos sobrescribir se encuentra dentro del mismo STACK.
>No importa la posicion de la memoria en el que arranque el STACK, nos movemos por dentro de él.
>Vamos a poder sobrescribir el valor de retorno sin tocar posiciones de memoria invalidas (solo tocamos posiciones del STACK).

(b) Utilizar el control del valor de retorno de una función para saltar a código externo introducido en un buffer en stack controlado por el usuario.

>Este enunciado se concentra en el hecho de que se salta a un codigo externo
>
>Stack Randomization afecta este ataque.
>Saltar en este caso involucra indicarle al sistema la posición de memoria donde se encuentra el codigo a ejecutar.
>Debido a que el código externo se encuentra en un Stack, Stack Randomization no permite conocer la posicion de memoria donde saltar.
>No se puede saltar al codigo externo gracias al Stack Randomization.

(c) Utilizar el control del valor de retorno de una función para ejecutar una syscall particular (por ejemplo read) que fue usada en otra parte del programa.

>Este enunciado se concentra en el hecho de que se salta a un syscall
>
>Stack Randomization NO afecta este ataque.
>Debido a que no queremos saltar a un posición aleatoria.
>La posiciones de memoria de la syscalls pueden ser conocidas y no pueden ser Randomizadas.
>Eso significa que sabemos perfectamente la posición de memoria a la que saltar a pesar de que los STACK se encuentren randomizados.

## 8) Command Injection
Suponiendo que el usuario controla el parámetro dir, el siguiente código, que corre con mayor nivel de privilegio, intenta mostrar en pantalla el directorio dir usando el comando ls.

	#define BUF_SIZE 1024
	void wrapper_ls(const char * dir) {
	   char cmd[BUF_SIZE];
	   snprintf(cmd, BUF_SIZE-1, "ls %s", dir);
	   system(cmd);
	}

(a) Muestre un valor de la cadena dir que además de listar el directorio actual muestre el contenido del archivo /etc/passwd.

>Podemos usar el valor: directorioSeñuelo; cat /etc/passwd

(b) Posteriormente se reemplazó esta por la función secure_wrapper_ls donde el tercer parámetro de snprintf en vez de ser "ls %s" se reemplaza por "ls \" %s\"". Muestre que la modificación no soluciona el problema de seguridad.

>Podemos incorporar las doble comillas en nuestro input: directorioSeñuelo"; cat /etc/passwd; ls "directorioSeñuelo

(c) Posteriormente se agregó una verificación de la cadena dir: no puede contener ningún carácter“;”. Muestre que esta modificación tampoco soluciona el problema.

>Podemos utilizar & en lugar de ; para inyectar un comando. /etc/passwd" && cat /etc/passwd "

(d) Proponga una versión de esta función que no tenga problemas de seguridad.

>Agregaria una lista de caracteres prohibidos: ['&', ';', '|', '"']
>
>También se podría agregar una lista de palabras prohibidas que incluya los comandos mas comunes de inyeccion: ["ls", "whoiam", "cat"]

## 9) Integer Overflow
La siguiente es una función que determina si una posición de memoria addr se encuentra dentro de una página de memoria page_addr.
Consideraciones: 

- Las páginas están alineadas a direcciones que terminan en bits_per_page bits en 0 
- Tienen un espacio de direcciones de esa misma cantidad de bits.

Ejemplo:

- La llamada addr_in_page(0x12345000, 12, 0x12345ABC) devuelve verdadero
- La dirección 0x12345ABC se encuentra en la página de 4 KB (12 bits de direcciones) que comienza en 0x12345000

Es un esquema típico en la arquitectura x86. 
Notar que del primer parámetro se ignoran los bits menos significativos, usualmente utilizados para atributos.


    bool addr_in_page(int page_addr, int bits_per_page, int addr) {
        int mask = (-1) << bits_per_page;
        return ((page_addr & mask) == (addr & mask));
    }

Si una dirección de memoria se encuentra dentro de una página alineada a b bits, se encuentra dentro de una página de dirección equivalente pero de más de b bits. Sin embargo, dadas tres variables controladas por el usuario:

- int paddr
- b
- addr

Mostrar valores para estas variables que hagan verdadera la siguiente evaluación:

	(addr_in_page(paddr, b, addr) && !addr_in_page(paddr, b+1, addr))

>No importa mucho los valores paddr y addr. El valor b es el que nos permite romper está función.
>
>Recordemos que las variables tienen cierto tamaño predefinido, incluyendo aquellas del tipo int.
>Si nos pasamos de ese tamaño, los bits excesivos no seran tomados en cuenta, provocando que un numero muy grande pueda ser tomado como uno muy pequeño.
>
>Esto se conoce como Integer Overflow. Ejemplo: 1000054 si no acepta el 1 por el tamaño de la variable, entonces el numero quedaría guardado como 54.Eso significa que si escogemos b igual al maximo integer que puede entrar en memoria de una variable int... Cuando hagamos b + 1, vamos a terminar con que el resultado es menor a b (ya que pasamos a tener un numero negativo por overflow). Especificamente b seria tomado como 0.
>
> Debido entonces a que b + 1 es menor a b, puede pasar que la dirección no este en paginas alineadas a b y a b+1 bits al mismo tiempo.

## 10) Integer Overflow
Suponiendo que el usuario controla la entrada estándar, el siguiente código tiene problemas de seguridad.

	#define BUF_SIZE 1024
	int suma_indirecta(void) {
	   int buf[BUF_SIZE];
	   int i, v;
	   memset(buf, 0, sizeof(buf));
	   while (cin >> i >> v) { // Leo el índice y el valor
	      if (i == -1) break; // Un índice -1 significa que tengo que terminar.
	      if (i < BUF_SIZE) buf[i] = v; // Guardo el valor en el buffer
	   }
	   // Calculo la suma de los valores
	   v = 0
	   for (i=0; i < BUF_SIZE; i++)
	      v += buf[i];
	   return v;
	}

(a) El código verifica que el valor de i no se pase del tamaño del buffer (BUF_SIZE). ¿Es suficiente esta verificación para garantizar que no se escribe fuera de buf?

>No es suficiente.
>Supongamos que tengo i = -2
>- i == -1 es false  --->  No se produce el break
>- i < BUF_SIZE  es true  --->  Se escribe en la posición -2 del buffer
>
>Observación: En C buf[2] = buf[-2].
>Sabiendo esto i = -2 tal vez no escriba fuera del buffer peeero...
>Si escogemos un número negativo 'n' tal que |n| > BUF_SIZE  ---->  Escribiremos fuera del buffer.

(b) Considerando que la dirección de retorno de esta función (suma_indirecta) se encuentra en una posición de memoria más alta (mayor) que buf, ¿existe algún valor de i que permita sobreescribirla al ejecutar el cuerpo del while? Justifique. Pista: Pensar en la aritmética de punteros que se realiza dentro del cuerpo del while.

>Existe: Un número negativo $n$ tal que |n| = BUF_SIZE + la distancia entre el BUFFER y la dirección de retorno.

(c) Si el compilador protegiera el stack colocando un canario de valor desconocido (incluso random), una posición de memoria antes (una posición menor) del return address de cada función, ¿aún es posible modificar el retorno de la función suma_indirecta y retornar de la misma satisfactoriamente?

>¯\_(ツ)_/¯

## 11) String Format Attack 
Suponiendo que el usuario controla la entrada estándar, el siguiente código tiene problemas de seguridad.

	#define MAX_BUF 4096
	void saludo(void) {
	   char nombre[MAX_BUF];
	   printf("Ingrese su nombre: ");
	   fgets(nombre, MAX_BUF, stdin);
	   printf(nombre);
	}

(a) ¿Dónde se introduce el problema de seguridad?

>Usando el printf podriamos usar un input malicioso que saque provecho del string format.
>
>Ejemplo: Si el usuario utiliza un input del estilo "Hello %s%s%s", cada uno de los "%s" sera interpretado como un puntero a un String.
>Usando muchos "%s" podriamos llenar el Stack y ocasionar un Segmentation Fault, produciendo que corte el programa.

(b) ¿Qué datos del programa (registros, variables, direcciones de memoria) pueden ser controladas por el usuario?

>El usuario podria controlar el stack al introducir punteros a traves del string forma con %n o %s.

(c) ¿Es posible sobreescribir la dirección de retorno a la que vuelve alguna de las llamadas a la función printf?

>Es posible, usando format string del estilo %x podemos movernos en el Stack hasta posicionarnos en la dirección de retorno. Una vez que sabemos que estamos encima de ella podemos usar [NUMERO]%n para escribirle la dirección de memoria que queramos.

(d) ¿Se soluciona el problema de seguridad si, luego del segundo printf, se coloca una llamada a exit(0)?

>No necesariamente, todavia podriamos escribir en el Stack o incluso en otras direcciones de memoria no protegidas. Además podriamos tirar el sistema al intentar escribir en direcciones de memoria protegidas. La solución es sanitizar el printf a traves del uso de %s.

## 12) String Format Attack Ext
Teniendo en cuenta el ejercicio anterior, considerar el siguiente código:

	void saludo(void) {
	   char nombre[80];
	   printf("Ingrese su nombre: ");
	   gets(nombre);
	   printf(nombre);
	   exit(0);
	}

Dado que la función saludo nunca retorna y, por ende, no interesa el valor de su return address, ¿es segura la segunda llamada a printf?

>No es segura, todavia podriamos tirar abajo el servicio al intentar escribir en posiciones de memoria protegidas.

## 14) Exception Vulnerability
En algunas combinaciones de sistema operativo y compilador, el siguiente código permite al usuario tomar control de la ejecución del programa:

	void leo_y_salgo(void) {
	   char leo[80];
	   gets(leo)
	   exit(1);
	}

Dado que al regresar de la función gets el programa termina la ejecución ignorando el valor de retorno de la función leo_y_salgo, para tomar control del programa se debe evitar volver de esta función.

Sabiendo que en estos sistemas al inicio del stack se almacena la dirección de los distintos handlers de excepciones del proceso (división por cero, error de punto flotante, etc.), explique cómo puede tomar control de la ejecución sin regresar de la función gets.

>Se puede lograr al cambiar la direccion de handleo de una de las excepciones y luego producir dicha excepcion. El programa intentara handlear la excepción usando una dirección alterada y terminara corriendo un codigo malicioso en su lugar