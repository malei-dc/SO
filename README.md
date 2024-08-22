# Bienvenidos
En cada práctica habrá un archivo .txt que, al final de la cursada, se pasó en limpio a un README para mayor claridad. Si estás en una PC, en el display del README (esquina superior derecha) hay un menú donde se mostrarán todas las secciones, que corresponden a cada ejercicio realizado. Aunque algunos ejercicios fueron consultados en clase, la gran mayoría están sujetos a errores y podrían no estar completamente justificados (especialmente los de protección y seguridad). 

Espero que les sea de ayuda y utilidad.

## Guía de ejercicios
- [Práctica 1:](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Practica1) Procesos y API del SO
- [Práctica 2:](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Practica2) Scheduling
- [Práctica 3:](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Practica3) Sincronización entre procesos
- [Práctica 4:](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Practica4) Administración de memoria
- [Práctica 5:](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Practica5) Entrada/Salida
- [Práctica 6:](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Practica6) Sistemas de archivos
- [Práctica 7:](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Practica7) Sistemas distribuidos
- [Práctica 8:](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Practica8) Protección y seguridad

## Clases [teóricas](https://github.com/malei-dc/SO/tree/main/Teoricas)
## Clases [prácticas](https://github.com/malei-dc/SO/tree/main/Practicas)
## Talleres
1. Taller de [syscalls y señales](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Talleres/taller1-syscall) (Completado)
2. Taller de [IPC](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Talleres/taller2-IPC) (Incompleto)
3. Taller de [drivers](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Talleres/taller3-drivers) (Completado)
4. Taller de [filessystem](https://github.com/malei-dc/SO/tree/main/Guia-Ejercicios/Talleres/taller4-filesSystem) (Completado)

## Programa de la materia
### Introducción
- Qué es un SO
- Introducción histórica
- Elementos básicos de un SO
- Manejo básico de un shell Unix

### Procesos y API
- Procesos 
- Estados de un proceso
- Introducción al scheduler
- E/S bloqueante / no bloqueante
- IPC (InterProcess Communication)

### Scheduling
- Objetivos de la política de scheduling
- Scheduling con y sin desalojo
- Políticas de scheduling
- Scheduling para tiempo real y para SMP

### Sincronización entre procesos
- Contención
- Condiciones de carrera
- Secciones críticas
- TestAndSet
- Busy waiting / sleep
- Productor - Consumidor
- Semáforos
- Introducción a deadlock
- Monitores
- Variables de condición

### Programación concurrente
- Algoritmos wait-free 
- Algoritmos lock-free
- CAS (Compare-And-Swap)
- Programación de multicores
- Invalidación de caché
- Reorden de instrucciones

### Administración de memoria
- Segmentación
- Paginación
- Swapping
- MMU
- Memoria virtual
- Copy-on-write
- Algoritmos de reemplazo de páginas

### Administración de entrada/salida
- Polling, interrupciones, DMA
- Almacenamiento secundario
- Drivers
- Políticas de scheduling de E/S a disco
- Gestión del disco (formateo, booteo, bloques dañados)
- RAID
- Copias de seguridad
- Spooling
- Clocks

### Sistemas de archivos
- Responsabilidades del FS
- Punto de montaje
- Representación de archivos
- Manejo del espacio libre
- FAT, inodos
- Atributos
- Directorios
- Caché
- Consistencia, journaling
-  Características avanzadas
-  NFS, VFS

### Protección y seguridad
- Conceptos de protección y seguridad
- Matrices de permisos
-  MAC vs. DAC
-  Autenticación, autorización y auditoría
-  Funciones de hash de una vía
-  Encriptación simétrica
-  RSA
-  Privilegios de procesos
-  Buffer overflows
-  Inyección de parámetros
- Condiciones de carrera
- Sandboxes
-  Principios generale de seguridad

### Sistemas distribuidos
- Taxonomía de Flynn
- Arquitecturas de HW y SW para sistemas distribuidos
-  RPC
-  Threads
-  Pasaje de mensajes
-  Orden parcial entre eventos
-  Livelock
-  Acuerdo bizantino
-  Intuición de safety, liveness, fairness
-  Algoritmo del banquero
-  Panadería de Lamport
-  Modelos de fallas y métricas de complejidad
-  Exclusión mutua y locks distribuidos
-  Elección de líder
-  Instantánea global consistente
-  2PC

### Conceptos avanzados
- Virtualización
- Contenedores
- Cloud computing

## Docentes
* Diego Slezak (Profesor)
* Rodolfo Sumoza (JTP)
* Fernando Schapachnik (JTP)
* Gisela Confalonieri (AY1)
* Hernán Gagliardi (AY1)
* David Venegas (AY1)
* Franco Pyrih (AY1)
* Tomás Chimenti (AY2)
* Debora Oca (AY2)

# Contribuir
Si ves un error, queres completar/corregir un ejercicio o compartir una mejor resolución en el repo, podes abrir un issue o hacer un pull request sin drama, o incluso me podes mandar mail y con gusto le hecho un ojo. :D
