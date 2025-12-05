# Readme ADA 04
## Explicación del ADA
Este ADA consistió en desarrollar un sistema de gestión de estudiantes utilizando un árbol binario de búsqueda (ABB), donde la matrícula funciona como clave para ordenar la información. Cada nodo almacena datos del estudiante (matrícula, nombre, apellido y calificaciones dinámicas). 

El sistema permite generar reportes ordenados por matrícula o promedio, filtrar estudiantes según su rendimiento, insertar y eliminar registros,
actualizar calificaciones, contar alumnos con notas menores a un valor dado y visualizar el árbol en formato ASCII.

### Cómo ejecutar el archivo

1. Extraiga los archivos en la misma carpeta.

2. Compile el archivo: student_manager.c
 Esto generará el ejecutable student_manager.exe (o el nombre que le dé al compilar).

3. Ejecute student_manager.exe (puede compilar y correr desde Visual Studio Code a la vez).

4. La terminal mostrará el menú principal, donde puede elegir entre:

	Agregar estudiante (1)

	Buscar estudiante (2)

	Ver reportes (3)

	Mostrar nodos en un nivel específico (4)

	Mostrar árbol ASCII (5)

	Salir (6)

4.1 Agregar estudiante (opción 1)

	-El programa le pedirá:

	-Matrícula (ID)

	-Nombre

	-Apellido

	-Primera calificación (opcional, si deja vacío será 0)

Al final, el estudiante se insertará automáticamente en el ABB por su matrícula.

4.2 Buscar estudiante (opción 2)

Submenú donde puede:

	1.- Agregar calificación

	2.- Actualizar calificación

	3. - Eliminar estudiante

	4.- Regresar al menú principal

4.2.1 Agregar calificación

	* El programa pedirá la calificación a añadir.

	* Actualiza el promedio automáticamente.

4.2.2 Actualizar calificación

	-- Se muestran todas las calificaciones con su índice.

	-- Se ingresa el índice a modificar y el nuevo valor.

	-- El promedio se recalcula automáticamente.

4.2.3 Eliminar estudiante

	- Se pide confirmación antes de eliminar.

	- Una vez eliminado, se regresa al menú principal.

4.3 Reportes (opción 3)

	* Listado ordenado por matrícula (ID)

	* Listado ordenado por promedio (descendente)

	* Filtrado por promedio (mayor o menor que un umbral ingresado)

4.4 Mostrar nodos en un nivel (opción 4)

	-- Se ingresa el nivel (0 = raíz)

	-- El programa muestra todos los estudiantes que están en ese nivel del ABB.

4.5 Mostrar árbol ASCII (opción 5)

	- Se imprime el ABB en formato estructurado, mostrando la jerarquía de estudiantes según su matrícula.

	- Esto permite visualizar el árbol completo de manera rápida y clara.

* * Nota * *

** Todos los datos se manejan dinámicamente en memoria.

** Se asegura liberar toda la memoria al salir del programa.

** La clave de búsqueda y ordenación del ABB es la matrícula del estudiante (ID).


** Comparaciones de promedio permiten ver cuántos estudiantes tienen mejor o peor promedio que un estudiante específico.

## Reflexión Personal
```
Trabajar en este programa me exigió analizar con detalle cómo se organizan los datos en un árbol binario.
Ese análisis me permitió comprender mejor la importancia de la estructura y la memoria dinámica en C.
Aunque fue retador, me dio claridad sobre cómo un buen diseño facilita reportes y búsquedas eficientes.
```
