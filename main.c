#include "testing.h"
#include <stdlib.h>
#include <stdio.h> 

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_abb_alumno(void);

int main(int argc, char *argv[])
{	
	
    printf("~~~ PRUEBAS CÁTEDRA ~~~\n");
    pruebas_abb_alumno();

    return failure_count() > 0;
}
