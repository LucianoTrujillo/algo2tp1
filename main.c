#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ioctl.h> //para obtener dimension de la consola
#include <stdarg.h>
#include <time.h>
#include "evento_pesca.h"
#include "interaccion_usuario.h"

#define ARRECIFE "arrecife.txt"
#define ACUARIO "acuario.txt"
#define ERROR -1
#define EXITO 0

//comandos
#define AYUDA "ayuda"
#define ES_RAPIDO "es_rapido"
#define ES_EXOTICO "es_exotico"
#define ES_PALINDROMO "es_palindromo"
#define COLOR_CONTIENE_Z "color_contiene_z"
#define PESO_ES_VEL_AL_CUADRADO "peso_es_vel_al_cuadrado"

#define VEL_RAPIDO 5
#define LONGITUD_NOMBRE_EXOTICO 15 

bool es_rapido(pokemon_t* pokemon){
	return pokemon->velocidad > VEL_RAPIDO; 
}

bool es_especie_exotica(pokemon_t* pokemon){
	return strlen(pokemon->especie) > LONGITUD_NOMBRE_EXOTICO; 
}

/*
	Devuelve si una palabra es palidnroma.
	Recibe la palabra que se quiere testear.
	Recibe desde que letra se desea comenzar, siendo 1 la primera.
*/
bool es_palindromo(char* palabra, size_t avanza){

	if(strlen(palabra) == 1){
		return true;
	}

	if(palabra[0] == palabra[strlen(palabra) - avanza]){

		return es_palindromo(palabra + 1, avanza + 1);
	}

	return false;
}

bool es_especie_palindromo(pokemon_t* pokemon){
	return es_palindromo(pokemon->especie, 1);
}

bool color_contiene_z(pokemon_t* pokemon){
	return strchr(pokemon->color, 'z');
}

bool el_peso_es_su_velocidad_al_cuadrado(pokemon_t* pokemon){
	return pokemon->peso == pokemon->velocidad * pokemon->velocidad;
}

void mostrar_pokemon(pokemon_t* pokemon){
	c_print("Especie: %s, Velocidad: %i, Peso: %i, Color: %s\n",
		 	pokemon->especie,
		 	pokemon->velocidad,
		 	pokemon->peso,
		 	pokemon->color);
}

void mostrar_ayuda(){
	c_print("-----------------------------------------------------------------------------------------------------------------------------------------\n");
	c_print("Bienvenido al evento acuático mas grande del mundo pokemon.\n");
	c_print("El programa consiste en trasladar los pokemon actuaticos desde el arrecife hasta el acuario.\n");
	c_print("Los pokemon del arrecife se deben encontrar en el archivo arrecife.txt y los pokemones del acuario se encontrarán en acuario.txt\n");
	c_print("Para lograrlo, necesitas pasar que características deben cumplir los pokemones trasladados junto con la cantidad que se desea trasladar.\n");
	c_print("De no llegar a la cantidad solicitada por usted, no se hará ningún translado\n");
	c_print("-----------------------------------------------------------------------------------------------------------------------------------------\n\n");
	c_print("Utilización del programa:\n");
	c_print("luego de escribir el nombre del programa, inserte las características que deben cumplir los pokemones de la siguiente manera: \n");
	c_print("./evento_pesca <característica_1>=<cantidad_pokemones_de_esta_caracteristica> <característica_n>=<cantidad_pokemones_de_esta_caracteristica> ... \n");
	c_print("las características disponibles por el momento son: \n");
	c_print(" - es_rapido: su velocidad supera los 5 puntos\n");
	c_print(" - es_especie_exotica: su especie supera los 15 caracteres de longitud\n");
	c_print(" - es_especie_palindromo: su especie es una palabra palindroma (https://en.wikipedia.org/wiki/Palindrome)\n");
	c_print(" - color_contiene_z: su color contiene la letra z\n");
	c_print(" - el_peso_es_su_velocidad_al_cuadrado: ¿...hace falta que aclare?\n\n");
	c_print("Un ejemplo de uso sería: ./evento_pesca es_rapido=3 es_especie_palindromo=1 color_contiene_z=3\n");
	c_print("-----------------------------------------------------------------------------------------------------------------------------------------\n\n");
	c_print("Para ver este mensaje nuevamente, escriba 'ayuda' luego del nombre del programa.\n\n");
	c_print("-----------------------------------------------------------------------------------------------------------------------------------------\n\n");
}

int main(int argc, char**argv) {

	if(argc == 2 && strstr(argv[1], AYUDA)){
		mostrar_ayuda();
		return EXITO;
	}

	acuario_t* acuario = crear_acuario();

	if(!acuario){
		return ERROR;
	}

	arrecife_t* arrefice = crear_arrecife(ARRECIFE);

	if(!arrefice){
		liberar_acuario(acuario);
		return ERROR;
	}
	
	printf("\n\n");
	c_print("Los pokemones inicialmente en el arrecife: \n\n");
	censar_arrecife(arrefice, mostrar_pokemon);

	bool existe_caracteristica = false;

	for(int i = 1; i < argc; i++){
		int cantidad_seleccion = atoi(strstr(argv[i], "=") + 1);
		int estado_traslado = EXITO;

		if((existe_caracteristica = strstr(argv[i], ES_RAPIDO))){
			estado_traslado = trasladar_pokemon(arrefice, acuario, es_rapido, cantidad_seleccion);
		} else if((existe_caracteristica = strstr(argv[i], ES_EXOTICO))){
			estado_traslado = trasladar_pokemon(arrefice, acuario, es_especie_exotica, cantidad_seleccion);
		} else if((existe_caracteristica = strstr(argv[i], ES_PALINDROMO))){
			estado_traslado = trasladar_pokemon(arrefice, acuario, es_especie_palindromo, cantidad_seleccion);
		} else if((existe_caracteristica = strstr(argv[i], COLOR_CONTIENE_Z))){
			estado_traslado = trasladar_pokemon(arrefice, acuario, color_contiene_z, cantidad_seleccion);
		} else if((existe_caracteristica = strstr(argv[i], PESO_ES_VEL_AL_CUADRADO))){
			estado_traslado = trasladar_pokemon(arrefice, acuario, el_peso_es_su_velocidad_al_cuadrado, cantidad_seleccion);
		}
	
		if(existe_caracteristica){
			printf("\n\n");
			if(estado_traslado == ERROR){
				c_print("El traslado no se pudo realizar!\n");
				return ERROR;
			} else {
				c_print("Luego del traslado nº%i, %s\n\n", i, argv[i]);
				censar_arrecife(arrefice, mostrar_pokemon);
			}
		} else {
			printf("\n\n");
			c_print("La catacteristica numero nº%i, %s no existe.\n\n", i, argv[i]);
		}
	}

	c_print("Simulación terminada.\n");
	guardar_datos_acuario(acuario, ACUARIO);
	liberar_acuario(acuario);
	liberar_arrecife(arrefice);
	return EXITO;
}
