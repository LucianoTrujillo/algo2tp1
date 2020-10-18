# Evento acuático con memoria dinámica

## Introducción

Se desea crear una API (librería) que contenga las funcionalidades para poder simular el traslado de pokemon desde un arrecife hasta un acuario.<br /> 
Los pokemon del arrecife se deben encontrar un archivo de texto con formato `especie1;velocidad1;peso1;color1` y los pokemones del acuario se encontrarán en `acuario.txt` con el mismo formato. En el caso de no encontrar el archivo de arrecife pasado por argumento, se usará el default: "arrecife.txt"<br />
Para lograrlo, necesitas pasar por argumentos del programa, el nombre del archivo que contiene a los pokemon del arrecife y que características deben cumplir los pokemones trasladados junto con la cantidad que se desea trasladar.<br />
De no llegar a la cantidad solicitada por usted, no se hará ningún translado.<br />


## Compilación

Para comilarlo y testear los leaks de memoria con valgrind, basta con hacer: <br/>
`$ make all`
Para compilar solamente: <br/>
`$ make evento_pesca`
La información sobre cada paso se encuentra en el archivo `makefile`<br/>

## Ejecución

luego de escribir el nombre del programa (`./evento_pesca`), inserte el nombre del archivo que contiene a los pokemon de la siguiente manera: <br/>
`arrecife=<nombre del archivo>` <br/>
Luego, inserte las características que deben cumplir los pokemones de la siguiente manera: <br/>
`<característica 1>=<cantidad pokemones de caracteristica 1> <característica n>=<cantidad pokemones de esta caracteristica> ...` <br/>
las características disponibles por el momento son: 
 - `es_rapido`: su velocidad supera los 5 puntos.
 - `es_especie_exotica`: su especie supera los 15 caracteres de longitud.
 - `es_especie_palindromo`: su especie es una palabra palindroma (https://en.wikipedia.org/wiki/Palindrome).
 - `color_contiene_z`: su color contiene la letra z.
 - `el_peso_es_su_velocidad_al_cuadrado`: ¿...hace falta que aclare?<br />
 
Un ejemplo de uso sería: `./evento_pesca arrecife=arrefice.txt es_rapido=3 es_especie_palindromo=1 color_contiene_z=3`<br />
Para ver este mensaje nuevamente, escriba 'ayuda' luego del nombre del programa.

## Conceptos teóricos para realizar el tp

### Punteros
  Un tipo de variable que contiene una única dirección de memoria de otra variable. <br />
  Almacena direcciónes de variables de todo tipo, ya sean datos primitivos, estructuras complejas, funciones, hasta otro punteros. <br />
  Una vez guardada alguna dirección, puede *desreferenciarse*, es decir acceder al valor de aquella variable a la cual apunta. Al desreferenciar puede alterar el valor de esa variable.<br />
  Consta de un tipo para que el compilador sepa como se debe comportar la aritmética de ese puntero. (Con el tipo de variable al que apunta, puede saber cuandos bytes en memoria debe moverse para ir de un elemento a otro).<br />
  Sin embargo, existe la posibilidad de no especificarle un tipo especifico con la palabra reservada *void*, para que el tipo de variable al cual referencie pueda variar.<br /> 

### Aritmética de punteros

Los operadores de los punteros se pueden separar en 3: <br/><br/>
`&`: utilizado para desreferenciar variables, es decir obtener la dirección de memoria de ésta. <br/> Esta dirección puede luego asignarsela a un puntero de la siguiente forma: `int* ptr = &numero;` <br/><br/>
`*`: utilizado para acceder al valor de la variable al cual apunta el puntero. <br/> Puede utilizarse de la siguiente manera: `int numero = *ptr;` <br/><br/>
`+/-`: utilizados para acceder al elemento posterior y anterior respectivamente que apunto el puntero. <br/> Dado que para este tipo de operaciones siempre hay queusar punteros tipados, el compilador se encarga de saber cuantos bytes moverse para no acceder de manera errónea a los datos. <br/> Puede utilizarse de la siguiente manera: `int siguiente_numero = *(ptr+1);`. Utilizar la sintaxis de arreglos es totalmetne válida: `int siguiente_numero = ptr[1];` <br/><br/>


### Punteros a funciones

Un puntero a una función almacena la dirección de memoria de esta función. Permite pasarla entre funciones, usarlas en vectores, usarla como atributos de registros y ejecutarlas. Su sintaxis no es la más bonita pero resultan muy útiles. <br/> 
Para declarar punteros a funciones se utiliza la siguiente sintaxis: <br/>
`tipo_retorno (*nombre_puntero_a_funcion) (tipo argumento_1, tipo argumento_2, ... tipo argumento_n);` <br/>
Luego simplemente podemos asignar las referencias de las funciones a nuestro puntero de funcion de la siguiente manera: 
`nombre_puntero_a_funcion = funcion_x;`<br/>
Y llamarlas como una funcion cualquiera: <br/>
`nombre_puntero_a_funcion(argumento_1, argumento_2, ... argumento_n);`<br/>


### Malloc y Realloc

`malloc`es una función de la biblioteca estándar de C que permite reservar memoria dinámica (heap) para ser utilizada en nustro programa. Una vez que no se requiere mas la memoria reservada, el programador debe encargarse de liberarla para que no se acumule.<br/>
Es de utilidad para cuando se quieren manejar datos de los cuales no se sabe el tamaño de antemano, y para crear variables en funciones y que perduren luego de que estas sean movidas del stack de ejecución. <br/>
`malloc`recibe la cantidad de bytes que se desean reservar y devuelve un puntero a la primer posición del bloque reservado. En caso de fallar por diferentes motivos, devuelve el direcciñon nula (`null`), la cual no es utilizable. <br/>
Un breve ejemplo de su uso es: `int* lista_de_cinco_enteros = malloc(sizeof(int) * 5);`. <br/>
Es importante verificar que no haya devuelo la dirección nula, ya que en caso de tratar de acceder a esta, el programa fallará. <br/><br/>

`realloc` trata de modificar el bloque de memoria dinámica pasado por parámetros. Tratará de agrandar o achicar el bloque en `n` cantidad de bytes pasado por parámetros. <br/>
En el mejor de los casos, si el tamaño que se desea agrandar entra en bloques consecutivos de memoria donde se encuentra el contenido actual, devuelve la misma dirección de memoria pasada por parámetros, pero ahora con mas espacio. <br/>
Si el tamaño que se desea agrandar supera al tamaño disponible consecutivo, se devuelve otro lugar en memoria capaz de almacenar el contenido actual mas el nuevo tamaño que se desea disponibilizar.  <br/>
Si no se encuentra ningún espacio disponible, devolvera NULL y el programa debe finalizarse. <br/>
Si se quiere achicar el espacio y la operación es exitosa, devuelve la dirección pasada por parámetros liberando el espacio que ya no es utilizado.
En caso de querer achicarse y no lograrlo, se devolverá NULL. <br/>
Un breve ejemplo: `char* ptr_20_caracteres = realloc(ptr_10_caracteres, 20);`<br/>

Para liberar la memoria utilizada, se de hacer uso de la función `free` de la librería estándar, a la cual se le pasa un puntero que apunta a un bloque de memoria en el heap. Esta función se encarga de *"devolver"* al sistema operativo esa memoria para poder ser utilizada. Un ejemplo de su uso: `free(lista_de_cinco_punteros);`.<br/> Notemos que libera bloques de memoria, no hace falta liberar cada entero por separado.

