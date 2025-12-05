# Readme ADA 07

## Explicación del ADA
El ADA consistió en implementar en C/C++ dos aplicaciones que ponen en práctica estructuras de datos avanzadas:

### Registro de Estudiantes con Colisiones Controladas

- Se diseñó un sistema para universidades que almacena estudiantes usando su matrícula como clave.
- La estructura principal fue una tabla hash, 
con direccionamiento abierto (sondeo lineal o cuadrático) para manejar colisiones.
- Se implementaron funcionalidades básicas: agregar, buscar y eliminar estudiantes.

### Tabla de símbolos en un compilador

- Se construyó una tabla hash para registrar identificadores
  (variables, funciones, etc.) junto con sus atributos (tipo, valor, ámbito).

- El manejo de colisiones se realizó mediante listas ligadas, lo que permite búsquedas rápidas
  y eficientes en el contexto de compiladores.


## readme Aplicación 1
Compilar el programa

Abrir la terminal en Visual Studio Code y moverse a la carpeta donde está guardado el archivo:
```
cd "C:\<ruta>\ADA07_E02"
```
Compilar el programa con:
```
gcc ADA07_AP02.c -o ADA07_AP02.exe
```
Cómo ejecutar el programa

Después de compilar, ejecutar con:
```
.\ADA07_AP02.exe
```
## 1. Como usar el código:
```
Crea una tabla hash de tamaño 120.

Inserta tres o más estudiantes (Juan Pérez, Miguel Gómez y Ana López).

Busca un estudiante por matrícula.

Intenta eliminar un estudiante.

Muestra los resultados de búsqueda y eliminación en pantalla.
```
## 2. Estructuras usadas

Estudiante: contiene matrícula, nombre y carrera.

Tablahash: contiene un arreglo de estudiantes y otro de estados para manejar colisiones.

## 3. Funciones implementadas

inicializarTabla(): Inicializa la tabla hash.

insertar(): Inserta un estudiante usando sondeo lineal.

buscar(): Busca un estudiante por matrícula.

eliminar(): Elimina un estudiante marcándolo como borrado.

```
-> Nota <-

El programa ya incluye los datos de prueba en main(), 
por lo que se ejecuta directamente sin pedir datos al usuario.
```

## readme Aplicación 2
Instrucciones para compilar y correr:

Moverse a la carpeta de la tarea en la terminal de Visual Studio Code:
```
cd "C:\<Ruta donde la carpeta descomprimida es>\ADA07_E02\ADA07_AP02"
```
Comando para compilar:
```
g++ -std=c++11 -g ADA07_AP02.cpp hash_table.cpp -o ADA07_AP02.exe
```
Comando para correr el codigo:
```
.\ADA07_AP02.exe
```
Como usar el código:
```
1.- "Add Identifier" te dejara agregar un identificador al archivo. te preguntara primero por la clave/Identificador de la variable, y luego por su ámbito.
    si ya existe un identificador con esa variable, no se te dejara crear el nuevo identificador.
      En caso de ser valido, podras ingresar su tipo y su valor (Si nada es puesto, nada será asignado).
2.- "Search Identifier" puedes buscar un identificador en el archivo. de encontrarse, se desplegara. en caso de multiples identificadores, con ambito diferente,
    se desplegaran y se podra elegir cual se quiere obtener mas detalles, incluyendo el numero de colisiones que esa tuvo;
    en caso de no encontrar ninguno, se mandara error y regresara al menu principal.
3.- "Delete Identifier" es similar a la busqueda, solo que te pedira confirmar si quieres borrarlo.
4.- "Display All Identifiers" desplega todos los identificadores, agrupados por index, así como sus detalles.
5.- "Exit" Cierra el programa.
```
Ahora, tambien se pueden agregar Funciones (vacias, esencialmente variables con diferente tipo) y Variables desde el identifiers.txt

Sin embargo, debe seguir la syntaxis correcta:
```
<"Global" o "Local"> <Tipo de variable> <ID> = <Valor>;
```
(el programa automáticamente ordenara el ID de acuerdo a su Index, y calculara las colisiones necesarias para añadirlo en ese momento.)

Ejemplo:

Global int y = 2;

Pasara a ser:

Global int y = 2 #collisions=0;

cuando el código lea el archivo

# Reflexión Personal 
El ADA me permitió comprender mejor cómo las estructuras de datos se aplican en problemas reales, desde la gestión de 
estudiantes hasta el funcionamiento interno de un compilador, Aunque enfrenté dificultades al manejar colisiones y al organizar la memoria dinámica, 
fue una experiencia enriquecedora porque me obligó a analizar con detalle la lógica de cada implementación.
