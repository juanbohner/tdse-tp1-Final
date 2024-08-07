/********************** inclusions *******************************************/
#include "display.h"
#include "menu.h"
#include "main.h"
#include <stdbool.h>
#include <stdio.h>

/********************** arm_book Defines *******************************/
//#include "arm_book_lib.h"
// Functional states
#ifndef OFF
#define OFF    0
#endif
#ifndef ON
#define ON     (!OFF)
#endif

// Electrical states
#ifndef LOW
#define LOW    0
#endif
#ifndef HIGH
#define HIGH   (!LOW)
#endif


#define EMPTY_STR 	"                    "
/********************** macros and definitions *******************************/

void imprimir_menu_0(int next_hop){

	displayCharPositionWrite ( 0, next_hop);
	displayStringWrite( ">" );

    displayCharPositionWrite ( 1,0 );
	displayStringWrite( "Pagina 1           " );

	displayCharPositionWrite ( 1,1 );
	displayStringWrite( "Pagina 2           " );

	displayCharPositionWrite ( 1,2 );
	displayStringWrite( "Pagina 3           " );

	displayCharPositionWrite ( 0,3 );
	displayStringWrite( EMPTY_STR );
}

void imprimir_menu_1(){

	displayCharPositionWrite ( 0, 3);
	displayStringWrite( ">" );

    displayCharPositionWrite ( 0,0 );
	displayStringWrite( "Pagina 1           " );

	displayCharPositionWrite ( 0,1 );
	displayStringWrite( EMPTY_STR );

	displayCharPositionWrite ( 0,2 );
	displayStringWrite( EMPTY_STR );

	displayCharPositionWrite ( 1,3 );
	displayStringWrite( "Volver menu 0      " );
}

void imprimir_menu_2(int next_hop){

	displayCharPositionWrite ( 0, next_hop);
	displayStringWrite( ">" );

    displayCharPositionWrite ( 1,0 );
	displayStringWrite( "Prender LED        " );

	displayCharPositionWrite ( 1,1 );
	displayStringWrite( "Apagar LED         " );

	displayCharPositionWrite ( 0,2 );
	displayStringWrite( EMPTY_STR );

	displayCharPositionWrite ( 1,3 );
	displayStringWrite( "Volver menu 0      " );
}

void floatToStr(float num, char* str) {
    const char* prefix = "TempMicro= ";  // El prefijo que queremos añadir
    int i = 0;

    // Copiar el prefijo en str
    while (prefix[i] != '\0') {
        str[i] = prefix[i];
        i++;
    }

    // Después del prefijo, continuar con la conversión del número flotante
    int entero = (int)num;                     // Parte entera
    int decimal = (int)((num - entero) * 100); // Obtener dos decimales

    // Convertir parte entera a string
    if (entero == 0) {
        str[i++] = '0';
    } else {
        if (entero < 0) {
            str[i++] = '-';
            entero = -entero;
        }
        int rev = 0;
        while (entero > 0) {
            rev = rev * 10 + entero % 10;
            entero /= 10;
        }
        while (rev > 0) {
            str[i++] = (rev % 10) + '0';
            rev /= 10;
        }
    }

    // Añadir el punto decimal
    str[i++] = '.';

    // Convertir los decimales a string
    int dec1 = decimal / 10;  // Primer decimal
    int dec2 = decimal % 10;  // Segundo decimal

    str[i++] = dec1 + '0';
    str[i++] = dec2 + '0';

    // Rellenar el resto de str con espacios
    while (i < 19) {  // 39 para dejar el último espacio para '\0'
        str[i++] = ' ';
    }

    // Colocar el carácter nulo al final
    str[i] = '\0';
}


void imprimir_menu_3(int next_hop, float temp){

	char str[20];

	floatToStr(temp, str);

	//sprintf(str, "Temp micro = %.2f", temp);

	displayCharPositionWrite ( 0, next_hop);
	displayStringWrite( ">" );

    displayCharPositionWrite ( 1,0 );
	displayStringWrite(str);

	displayCharPositionWrite ( 1,1 );
	displayStringWrite( "3.2                " );

	displayCharPositionWrite ( 1,2 );
	displayStringWrite( "3.3                " );

	displayCharPositionWrite ( 1,3 );
	displayStringWrite( "Volver menu 0      " );
}

