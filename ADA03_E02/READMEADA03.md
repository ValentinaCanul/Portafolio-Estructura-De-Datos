# Readme ADA 03
Como ejecutar el archivo:

1- Extraiga los archivos en la misma carpeta.

2- compile el archivo: ADA03_E02.c
	Esto le creara el archivo ADA04_E02.exe

3- Ejecute ADA03_E02.exe (puede compilar y correr desde Visual Studio Code
a la vez.)

4- la terminal le pedirá la opción a realizar, entre modificar la lista(1), buscar una pelicula en especifico(2), 
Ver todas las peliculas(3), o salir(0)

 - 4.1 En modificar, podemos insertar una nueva pelicula(1), borrar una pelicula(2) y Actualizar una pelicula(3).
También puedes regresar al menú principal(0)

  - 4.1.1 En insertar una pelicula, el programa le pedirá todos los datos que las demás peliculas tienen. si se le pide un
numero, pero pone un carácter no numero, le pedirá que ponga un numero. al final, le pedirá si la quiere insertar al principio(1), al final (2) o en un lugar en especifico(3), especificando cual. si pone un valor invalido, 
se añadirá al final de la lista por default.
```
NOTA: para toda búsqueda por titulo, siempre que usted use un fragmento de el titulo,
 el programa le podrá regresar un titulo que contenga ese fragmento, usualmente el primero de la lista que cumpla ese fragmento.
```
  - 4.1.2 En borrar, le pedirá buscar por titulo(1) o por id(2). una vez encontrada la pelicula, le mostrara sus datos antes de
pedir confirmación de borrarla.

  - 4.1.3 En actualizar podrá buscar la pelicula por titulo(1) o por id(2). una vez encontrada la pelicula, le pedirá un valor
correspondiente a un aspecto a modificar de la pelicula (1-23) o salir sin modificar(0)

4.2 Al buscar una pelicula en especifico, te pedirá buscarla por titulo(1) o por id(2), o regresar al menú principal(0) se 
mostrara la información de la pelicula, y luego regresaras a el menú de búsqueda. 

4.3 Ver todas las peliculas mostrara una tabla donde se desplegaran todas las peliculas, 10 a la vez. puedes ir a la 
siguiente(n) o anterior(p) pagina, escribir el Id de una pelicula para detalles(Id), o salir al menú principal(1)

5 Si aquí vas a detalles de una pelicula, podrás actualizarla o borrarla desde este mismo menú. actualizarla te 
llevara a 4.1.3, pero al terminar, regresaras a los detalles de la pelicula, ya actualizada. al borrarla, regresaras al 
menu principal.

# Reflexión Personal
Me costó bastante trabajo lograr que el código pudiera leer un CSV con tantos datos, porque no es nada sencillo manejar 
errores y formatos. Ese esfuerzo me hizo darme cuenta de la paciencia y precisión que requiere programar.


