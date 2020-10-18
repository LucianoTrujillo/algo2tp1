CFLAGS=-Wall -Werror -Wconversion -std=c99 -g -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes
OUTPUT=evento_pesca

all: evento_pesca mem_test

evento_pesca: interaccion_usuario.c evento_pesca.c main.c
	gcc *.c $(CFLAGS) -o $(OUTPUT)

#un caso sencillo para probar la memoria
mem_test: evento_pesca
	valgrind $(VFLAGS) ./$(OUTPUT) es_rapido=2

debug: evento_pesca
	gdb -tui $(OUTPUT)
