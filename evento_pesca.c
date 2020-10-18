#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "evento_pesca.h"

#define FORMATO_LEER_POKEMON "%100[^;];%i;%i;%100[^\n]\n"
#define FORMATO_IMPRMIR_POKEMON "%s;%i;%i;%s\n"
#define ERROR -1
#define EXITO 0
#define CANTIDAD_LECTURA 4
#define LECTURA "r"
#define ESCRITURA "w"

/*
	Lee un pokemon del archivo pasado por parámetros y lo guarda en pokemon
	Si lee el pokemon correctamente, aumenta en 1 la cantidad de pokemon y devuelve EXITO
	Si no lee el pokemon correctamente, devuelve ERROR
*/
int leer_pokemon(FILE* archivo, pokemon_t* pokemon, int* cantidad_pokemon){
	strcpy(pokemon->especie, "");
	strcpy(pokemon->color, "");
	pokemon->peso = 0;
	pokemon->velocidad = 0;

	int leidos = fscanf(
		archivo,
		FORMATO_LEER_POKEMON,
		pokemon->especie,
		&(pokemon->velocidad),
		&(pokemon->peso),
		pokemon->color);

	if(leidos == CANTIDAD_LECTURA){
		(*cantidad_pokemon)++;
		return EXITO;
	} else {
		return ERROR;
	}
}

/*
	Recibe la ruta para leer los pokemon y una variable donde guardará la cantida de pokemon que leyó.
	Si la operación es exitosa, Devuelve un vector de pokemon alocados dinámicamente.
	Si hubo un error al abrir el archivo o alocar memoria para los pokemon, devuelve NULL.
*/
pokemon_t* leer_pokemons(const char* ruta_archivo, int* cantidad_pokemon){
	FILE* archivo = fopen(ruta_archivo, LECTURA);
	if(!archivo){
		return NULL;
	}

	pokemon_t* pokemon = malloc(sizeof(pokemon_t));
	if(!pokemon){
		fclose(archivo);
		return NULL;
	}

	*cantidad_pokemon = 0;
	while (leer_pokemon(archivo, pokemon + *cantidad_pokemon, cantidad_pokemon) == EXITO) {
		size_t nuevo_tamanio = sizeof(pokemon_t) * (size_t)(*cantidad_pokemon + 1);
		pokemon_t* nuevo_pokemon = realloc(pokemon, nuevo_tamanio);

		if(!nuevo_pokemon){
			*cantidad_pokemon = 0;
			free(pokemon);
			fclose(archivo);
			return NULL;
		} else {
			pokemon = nuevo_pokemon;
		}
	}

	if(*cantidad_pokemon == 0){
		free(pokemon);
		fclose(archivo);
		return NULL;
	}

	fclose(archivo);
	return pokemon;
}

/*
	Imprime en el archivo los datos del pokemon pasado por parametro.
	Si no pudo imprimir correctamente, devuelve un numero negativo.
*/
int imprimir_pokemon(pokemon_t* pokemon, FILE* archivo){
	return fprintf(
		archivo,
		FORMATO_IMPRMIR_POKEMON,
		pokemon->especie, pokemon->velocidad,
		pokemon->peso, pokemon->color);
}

/*
	Devuelve si la cantidad de pokemones en el arrecife que cumplan con la condicion que impone la funcion cumple_condicion
	llega a la cantidad exigida por cant_seleccion. 
*/
bool hay_suficientes_pokemon(arrecife_t* arrecife, bool (*cumple_condicion) (pokemon_t*), int cant_seleccion){
	int cant_seleccionados = 0;
	pokemon_t* pokemon = arrecife->pokemon;
	
	for(int i = 0; i < arrecife->cantidad_pokemon && cant_seleccionados < cant_seleccion; i++){
		cant_seleccionados += cumple_condicion(pokemon);
		pokemon++;
	}

	return cant_seleccionados == cant_seleccion;
}

/*
	Remueve el pokemon que se encuentra en la posicion index del buffer del pokemon del arrecife.
	Deja la cantidad de pokemones actualizada.
	No asegura mantener el orden recibido.
	Si falla al realocar, devuelve ERROR.
	Si es exitoso en la eliminación, devuelve EXITO.
*/
int remover_pokemon_arrecife(arrecife_t* arrecife, int index){
	pokemon_t ultimo_pokemon = arrecife->pokemon[arrecife->cantidad_pokemon-1];
	arrecife->pokemon[index] = ultimo_pokemon;

	size_t nuevo_tamanio = sizeof(pokemon_t) * (size_t)(arrecife->cantidad_pokemon - 1);
	pokemon_t* nuevos_pokemon_arrecife = realloc(arrecife->pokemon, nuevo_tamanio);
	if(!nuevos_pokemon_arrecife && arrecife->cantidad_pokemon > 1){
		return ERROR;
	}

	arrecife->cantidad_pokemon--;
	arrecife->pokemon = nuevos_pokemon_arrecife;

	return EXITO;
}

/*
	Agrega el pokemon al final del buffer del pokemon del acuario. 
	Deja la cantidad de pokemones actualizada.
	Si falla al realocar, devuelve ERROR.
	Si es exitoso, devuelve EXITO.
*/
int agregar_pokemon_acuario(acuario_t* acuario, pokemon_t pokemon_seleccionado, int* cantidad_seleccionados){
	size_t nuevo_tamanio = sizeof(pokemon_t) * (size_t)(acuario->cantidad_pokemon + 1);
	pokemon_t* nuevos_pokemon_acuario = realloc(acuario->pokemon, nuevo_tamanio);
	if(!nuevos_pokemon_acuario){
		return ERROR;
	}
	nuevos_pokemon_acuario[acuario->cantidad_pokemon++] = pokemon_seleccionado;
	acuario->pokemon = nuevos_pokemon_acuario;
	(*cantidad_seleccionados)++;

	return EXITO;
}


arrecife_t* crear_arrecife(const char* ruta_archivo){
	arrecife_t* arrecife = malloc(sizeof(arrecife_t));
	if(!arrecife){
		return NULL;
	}

	arrecife->pokemon = leer_pokemons(ruta_archivo, &(arrecife->cantidad_pokemon));

	if(arrecife->pokemon == NULL){
		liberar_arrecife(arrecife);
		return NULL;
	}

	return arrecife;
}

acuario_t* crear_acuario (){
	acuario_t* acuario = malloc(sizeof(acuario_t));
	if(!acuario){
		return NULL;
	}
	acuario->pokemon = NULL;
	acuario->cantidad_pokemon = 0;

	return acuario;
}

void liberar_acuario(acuario_t* acuario){
	free(acuario->pokemon);
	free(acuario);
}

void liberar_arrecife(arrecife_t* arrefice){
	free(arrefice->pokemon);
	free(arrefice);
}

void censar_arrecife(arrecife_t* arrecife, void (*mostrar_pokemon)(pokemon_t*)){
	pokemon_t* pokemon = arrecife->pokemon;

	for(int i = 0; i < arrecife->cantidad_pokemon; i++){
		mostrar_pokemon(pokemon);
		pokemon++;
	}
}


int guardar_datos_acuario(acuario_t* acuario, const char* nombre_archivo){
	FILE* archivo = fopen(nombre_archivo, ESCRITURA);
	pokemon_t* pokemon = acuario->pokemon;

	if(!archivo){
		return ERROR;
	}

	for(int i = 0; i < acuario->cantidad_pokemon; i++){
		imprimir_pokemon(pokemon, archivo);
		pokemon++;
	}

	fclose(archivo);
	return EXITO;
}


int trasladar_pokemon(arrecife_t* arrecife, acuario_t* acuario, bool (*cumple_condicion) (pokemon_t*), int cant_seleccion){
	int cantidad_seleccionados = 0, i = 0, estado = EXITO;

	if(!hay_suficientes_pokemon(arrecife, cumple_condicion, cant_seleccion)){
		return ERROR;
	}

	while(i < arrecife->cantidad_pokemon && cantidad_seleccionados < cant_seleccion) {
		if(cumple_condicion(&(arrecife->pokemon[i]))){
			pokemon_t pokemon_seleccionado = arrecife->pokemon[i];
			estado = remover_pokemon_arrecife(arrecife, i);
			if(estado == ERROR){
				return ERROR;
			}
			estado = agregar_pokemon_acuario(acuario, pokemon_seleccionado, &cantidad_seleccionados);
			if(estado == ERROR){
				return ERROR;
			}
		} else {
			i++;
		}
	}

	return EXITO;
}
