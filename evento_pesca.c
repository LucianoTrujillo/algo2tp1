#include <stdio.h>
#include <stdlib.h>
#include "evento_pesca.h"

#define FORMATO_LEER_POKEMON "%[^;];%i;%i;%[^\n]\n"
#define FORMATO_IMPRMIR_POKEMON "%s;%i;%i;%s\n"
#define ERROR -1
#define EXITO 0
#define CANTIDAD_LECTURA 4

pokemon_t* leer_pokemons(FILE* archivo, int* cantidad_pokemon){

	pokemon_t* pokemon = malloc(sizeof(pokemon_t));
	if(!pokemon){
		return NULL;
	}

	size_t i = 0;
	while (fscanf(archivo,
			FORMATO_LEER_POKEMON,
			pokemon[i].especie,
			&(pokemon[i].velocidad),
			&(pokemon[i].peso),
			pokemon[i].color) == CANTIDAD_LECTURA) {

		i++;
		pokemon_t* nuevo_pokemon = realloc(pokemon, sizeof(pokemon_t) * (i + 1));
		if(!nuevo_pokemon){
			free(pokemon);
			return NULL;
		}

		pokemon = nuevo_pokemon;
	}
	*cantidad_pokemon = (int)i;
	return pokemon;
}

arrecife_t* crear_arrecife(const char* ruta_archivo){
	FILE* archivo = fopen(ruta_archivo, "r");
	if(!archivo){
		return NULL;
	}

	arrecife_t* arrefice = malloc(sizeof(arrecife_t));
	if(!arrefice){
		fclose(archivo);
		return NULL;
	}

	arrefice->pokemon = leer_pokemons(archivo, &(arrefice->cantidad_pokemon));
	fclose(archivo);

	if(arrefice->pokemon == NULL){
		arrefice->cantidad_pokemon = 0;
		return NULL;
	}

	return arrefice;
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
	FILE* archivo = fopen(nombre_archivo, "w");
	pokemon_t* pokemon = acuario->pokemon;

	if(!archivo){
		return ERROR;
	}

	for(int i = 0; i < acuario->cantidad_pokemon; i++){
		fprintf(archivo, FORMATO_IMPRMIR_POKEMON,
				pokemon->especie, pokemon->velocidad,
				pokemon->peso, pokemon->color);
		pokemon++;
	}

	fclose(archivo);
	return EXITO;
}

/*
	Verifica si hay suficientes pokemon para trasladar desde el arrefice, que cumplan con la condicion que impone la funcion seleccionar_pokemon. 
*/
bool hay_suficientes_pokemon(arrecife_t* arrecife, bool (*seleccionar_pokemon) (pokemon_t*), int cant_seleccion){
	int cant_seleccionados = 0;
	for(int i = 0; i < arrecife->cantidad_pokemon && cant_seleccionados < cant_seleccion; i++){
		cant_seleccionados += seleccionar_pokemon(&(arrecife->pokemon[i]));
	}

	return cant_seleccionados == cant_seleccion;
}

/*
	Remueve el pokemon que se encuentra en la posicion index del buffer del pokemon del arrecife. Deja la cantidad de pokemones actualizada.
	No devuelve la lista de pokemon con el mismo orden.
	Si falla al realocar, devuelve ERROR.
	Si es exitoso, devuelve EXITO.
*/
int remover_pokemon_arrecife(arrecife_t* arrecife, int index){
	pokemon_t ultimo_pokemon = arrecife->pokemon[arrecife->cantidad_pokemon-1];
	arrecife->pokemon[index] = ultimo_pokemon;

	pokemon_t* nuevos_pokemon_arrecife = realloc(arrecife->pokemon, sizeof(pokemon_t) * (size_t)(arrecife->cantidad_pokemon - 1));
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
	pokemon_t* nuevos_pokemon_acuario = realloc(acuario->pokemon, sizeof(pokemon_t) * (size_t)(acuario->cantidad_pokemon + 1));
	if(!nuevos_pokemon_acuario){
		return ERROR;
	}
	nuevos_pokemon_acuario[acuario->cantidad_pokemon++] = pokemon_seleccionado;
	acuario->pokemon = nuevos_pokemon_acuario;
	(*cantidad_seleccionados)++;

	return EXITO;
}

int trasladar_pokemon(arrecife_t* arrecife, acuario_t* acuario, bool (*seleccionar_pokemon) (pokemon_t*), int cant_seleccion){
	int cantidad_seleccionados = 0, i = 0;

	if(!hay_suficientes_pokemon(arrecife, seleccionar_pokemon, cant_seleccion)){
		return ERROR;
	}

	while(i < arrecife->cantidad_pokemon && cantidad_seleccionados < cant_seleccion) {
		if(seleccionar_pokemon(&(arrecife->pokemon[i]))){
			pokemon_t pokemon_seleccionado = arrecife->pokemon[i];
			if(remover_pokemon_arrecife(arrecife, i) == ERROR){
				return ERROR;
			}
			if(agregar_pokemon_acuario(acuario, pokemon_seleccionado, &cantidad_seleccionados) == ERROR){
				return ERROR;
			}
		} else {
			i++;
		}
	}

	return EXITO;
}