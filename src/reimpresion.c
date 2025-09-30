#include <stdio.h>
#include <string.h>
#include "reimpresion.h"
#include "utilidades.h"
#include "transacciones.h"

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

void ejecutar_reimpresion(SistemaFinanciero *sistema)
{
    if (sistema->cantidad == 0)
    {
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
        if (indice_actual > 0)
            printf("[a] Anterior ");
        if (indice_actual < sistema->cantidad - 1)
            printf("[d] Siguiente ");
        printf("[q] Salir\n");
        printf("Seleccione: ");

        opcion = getchar();
        limpiar_buffer_entrada();

        switch (opcion)
        {
        case 'a':
            if (indice_actual > 0)
                indice_actual--;
            break;
        case 'd':
            if (indice_actual < sistema->cantidad - 1)
                indice_actual++;
            break;
        case 'q':
            return;
        }
    } while (1);
}