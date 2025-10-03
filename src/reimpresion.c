#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "reimpresion.h"
#include "utilidades.h"
#include "transacciones.h"
#include "validaciones.h"

void mostrar_transaccion(Transaccion *transaccion, int indice, int total)
{
    char monto_str[20];
    char pan_enmascarado[20];
    char tipo_str[15];

    formatear_monto(transaccion->monto, monto_str, sizeof(monto_str));
    enmascarar_pan(transaccion->pan, pan_enmascarado);

    if (transaccion->tipo == COMPRA)
    {
        strcpy(tipo_str, "COMPRA");
    }
    else
    {
        strcpy(tipo_str, "ANULACION");
    }

    printf("=== TRANSACCION %d de %d ===\n\n", indice + 1, total);
    printf("Referencia: %s\n", transaccion->numero_referencia);
    printf("Tipo: %s\n", tipo_str);
    printf("Monto: %s\n", monto_str);
    printf("PAN: %s\n", pan_enmascarado);
    printf("Franquicia: %s\n", transaccion->franquicia);
    printf("Fecha/Hora: %s\n", transaccion->fecha_hora);
    printf("Expiracion: %s\n", transaccion->fecha_expiracion);
}

// Función auxiliar para mostrar las opciones disponibles
void mostrar_opciones_navegacion(int indice_actual, int total_transacciones)
{
    printf("Opciones: ");
    if (indice_actual > 0)
        printf("[a] Anterior ");
    if (indice_actual < total_transacciones - 1)
        printf("[d] Siguiente ");
    printf("[q] Salir\n");
}

void ejecutar_reimpresion(SistemaFinanciero *sistema)
{
    if (sistema->cantidad == 0)
    {
        printf("=== TRANSACCION ===\n\n");
        printf("No hay transacciones para mostrar.\n");
        presionar_para_continuar();
        return;
    }

    int indice_actual = sistema->cantidad - 1;
    char opcion;

    do
    {
        limpiar_consola();
        mostrar_transaccion(&sistema->transacciones[indice_actual], indice_actual, sistema->cantidad);

        printf("\nNavegacion: ");
        mostrar_opciones_navegacion(indice_actual, sistema->cantidad);

        // Usar la nueva función de validación (ya incluye validación de límites)
        opcion = validar_navegacion_reimpresion(indice_actual, sistema->cantidad);

        // Procesar la opción validada (ya viene validada, solo ejecutar)
        switch (opcion)
        {
        case 'a':
            indice_actual--; // Ya está validado que se puede mover hacia atrás
            break;
        case 'd':
            indice_actual++; // Ya está validado que se puede mover hacia adelante
            break;
        case 'q':
            // Reutilizar la función de confirmación para salir
            if (validar_confirmacion_sn("Esta seguro que desea salir de la reimpresion?") == 's')
            {
                return;
            }
            // Si el usuario dice 'n', continúa en el bucle
            break;
        }
    } while (1);
}