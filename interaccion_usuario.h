#ifndef _INTERACCION_USUARIO_H_
#define _INTERACCION_USUARIO_H_

/*
	Un custom delay para darle un efecto de pokemon a los mensajes del programa, recibe la cantidad de milisegundos que se desea esperar.
*/
void delay(int milisegundos);

/*
	Un custom print para escribir en la mitad de la pantalla, con delay entre las letras similar a los juegos arcade.
	Recibe los parametros de la misma manera que printf, el primer argumento es el string formato y el resto son las variables del tipo correspondiente, en orden.
*/
void c_print(char *fmt, ...);

#endif /* INTERACCION_USUARIO_H */
