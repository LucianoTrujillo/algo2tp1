#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ioctl.h> //para obtener dimension de la consola
#include <stdarg.h>
#include <time.h>
#include "interaccion_usuario.h"

#define MILISEGUNDOS_EN_SEGUNDO 1000
#define DELAY_LARGO 500
#define DELAY_MEDIO 100
#define DELAY_CORTO 1

void delay(int milisegundos)
{
    long pausa;
    clock_t ahora, antes;

    pausa = milisegundos*(CLOCKS_PER_SEC/MILISEGUNDOS_EN_SEGUNDO);
    ahora = antes = clock();
    while((ahora-antes) < pausa)
        ahora = clock();
}


void c_print(char *fmt, ...){
	va_list ap;
	char *p;
	int ival;
	char* txt;
	va_start(ap, fmt);
	struct winsize w; 
    ioctl(0, TIOCGWINSZ, &w); 
	printf("%*s> ", (int)((w.ws_col - strlen(fmt)) / 2), " ");
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			fflush(stdout);
			if(*p == ',' || *p == '.' || *p == '!' || *p == '?'){
				delay(DELAY_MEDIO); 
			} if(*p == '-'){
				delay(0); 
			} else {
				delay(DELAY_CORTO); 	
			}
			continue;
		}
		switch (*++p) {
			case 'd':
				ival = va_arg(ap, int);
				printf("%d", ival);
				break;
			case 'i':
				ival = va_arg(ap, int);
				printf("%i", ival);
				break;
			case 'c':
				ival = va_arg(ap, int);
				printf("%c", ival);
				break;
			case 's':
				txt = va_arg(ap, char*);
				printf("%s", txt);
				break;
			default: 
				putchar(*p);
				break;
		}
	}
	delay(DELAY_LARGO); 
}
