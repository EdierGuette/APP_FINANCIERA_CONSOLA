#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utilidades.h"

void limpiar_consola()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void presionar_para_continuar()
{
    printf("\nPresione Enter para continuar...");
    limpiar_buffer_entrada();
}

void enmascarar_pan(const char *pan, char *pan_enmascarado)
{
    int longitud = strlen(pan);
    if (longitud <= 8)
    {
        strcpy(pan_enmascarado, pan);
        return;
    }

    int asteriscos = longitud - 8;
    char mascara[20] = "";

    for (int i = 0; i < asteriscos; i++)
    {
        strcat(mascara, "*");
    }

    snprintf(pan_enmascarado, 20, "%.4s%s%.4s", pan, mascara, pan + longitud - 4);
}

long convertir_monto_a_centavos(const char *monto)
{
    char copia[20];
    strcpy(copia, monto);

    char *punto = strchr(copia, '.');
    long dolares, centavos = 0;

    if (punto)
    {
        *punto = '\0';
        dolares = atol(copia) * 100;
        centavos = atol(punto + 1);
    }
    else
    {
        dolares = atol(copia) * 100;
    }

    return dolares + centavos;
}

void formatear_monto(long centavos, char *buffer, int buffer_size)
{
    long dolares = centavos / 100;
    long cents = centavos % 100;
    snprintf(buffer, buffer_size, "$%ld.%02ld", dolares, cents);
}

void limpiar_buffer_entrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void eliminar_salto_linea(char *cadena)
{
    size_t len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n')
    {
        cadena[len - 1] = '\0';
    }
}

