#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "anulacion.h"
#include "utilidades.h"
#include "transacciones.h"

void ejecutar_anulacion(SistemaFinanciero *sistema)
{
    if (sistema->cantidad == 0)
    {
        printf("No hay transacciones para anular.\n");
        presionar_para_continuar();
        return;
    }

    char referencia[REF_LENGTH];
    char pan_verificacion[10];
    char cvv_verificacion[10];
    char opcion[10];

    printf("=== ANULACION DE COMPRA ===\n\n");
    printf("Ingrese 'q' en cualquier momento para cancelar\n\n");

    printf("Numero de referencia: ");
    if (!fgets(referencia, sizeof(referencia), stdin))
    {
        printf("Error al leer referencia.\n");
        presionar_para_continuar();
        return;
    }
    eliminar_salto_linea(referencia);

    // Verificar si quiere cancelar
    if (strcmp(referencia, "q") == 0 || strcmp(referencia, "Q") == 0)
    {
        printf("Anulacion cancelada por el usuario.\n");
        presionar_para_continuar();
        return;
    }

    Transaccion *transaccion = buscar_transaccion(sistema, referencia);

    // **QUITAR EL DEBUG O MOVERLO DESPUÉS DE LAS VALIDACIONES**
    if (!transaccion)
    {
        printf("ERROR: Numero de referencia no existe.\n");

        // DEBUG SOLO SI NO SE ENCUENTRA (opcional)
        printf("\nReferencias existentes:\n");
        for (int i = 0; i < sistema->cantidad; i++)
        {
            printf("- %s (%s)\n",
                   sistema->transacciones[i].numero_referencia,
                   sistema->transacciones[i].tipo == COMPRA ? "COMPRA" : "ANULADA");
        }
        printf("\n");

        presionar_para_continuar();
        return;
    }

    // **ESTA VALIDACIÓN SÍ DEBERÍA FUNCIONAR**
    if (transaccion->tipo == ANULACION)
    {
        printf("ERROR: La transaccion %s ya esta ANULADA.\n", transaccion->numero_referencia);
        presionar_para_continuar();
        return;
    }

    // DEBUG: Mostrar que encontró una transacción VÁLIDA
    printf("\n=== TRANSACCION ENCONTRADA ===\n");
    printf("Referencia: %s\n", transaccion->numero_referencia);
    printf("Tipo: COMPRA (vigente)\n");
    printf("Monto: $%ld.%02ld\n", transaccion->monto / 100, transaccion->monto % 100);
    printf("==============================\n\n");

    // Mostrar información de la transacción
    char monto_str[20];
    char pan_enmascarado[20];

    // Usar formatear_monto para mostrar correctamente
    formatear_monto(transaccion->monto, monto_str, sizeof(monto_str));
    enmascarar_pan(transaccion->pan, pan_enmascarado);

    printf("=== INFORMACION DE LA TRANSACCION ===\n");
    printf("Monto: %s\n", monto_str);
    printf("PAN: %s\n", pan_enmascarado);
    printf("Franquicia: %s\n", transaccion->franquicia);
    printf("=====================================\n\n");

    // Limpiar buffer
    limpiar_buffer_entrada();

    printf("Ultimos 4 digitos del PAN: ");
    if (!fgets(pan_verificacion, sizeof(pan_verificacion), stdin))
    {
        printf("Error al leer PAN.\n");
        presionar_para_continuar();
        return;
    }
    eliminar_salto_linea(pan_verificacion);

    if (strcmp(pan_verificacion, "q") == 0 || strcmp(pan_verificacion, "Q") == 0)
    {
        printf("Anulacion cancelada por el usuario.\n");
        presionar_para_continuar();
        return;
    }

    printf("CVV: ");
    if (!fgets(cvv_verificacion, sizeof(cvv_verificacion), stdin))
    {
        printf("Error al leer CVV.\n");
        presionar_para_continuar();
        return;
    }
    eliminar_salto_linea(cvv_verificacion);

    if (strcmp(cvv_verificacion, "q") == 0 || strcmp(cvv_verificacion, "Q") == 0)
    {
        printf("Anulacion cancelada por el usuario.\n");
        presionar_para_continuar();
        return;
    }

    // Validaciones del PAN
    if (strlen(pan_verificacion) != 4)
    {
        printf("ERROR: Debe ingresar exactamente 4 digitos.\n");
        presionar_para_continuar();
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        if (!isdigit(pan_verificacion[i]))
        {
            printf("ERROR: Solo se permiten digitos numericos.\n");
            presionar_para_continuar();
            return;
        }
    }

    int pan_longitud = strlen(transaccion->pan);
    char ultimos_cuatro_real[5];
    strncpy(ultimos_cuatro_real, transaccion->pan + pan_longitud - 4, 4);
    ultimos_cuatro_real[4] = '\0';

    if (strcmp(ultimos_cuatro_real, pan_verificacion) != 0)
    {
        printf("ERROR: Verificacion de PAN fallida.\n");
        printf("Se esperaba: %s\n", ultimos_cuatro_real);
        presionar_para_continuar();
        return;
    }

    if (strcmp(transaccion->cvv, cvv_verificacion) != 0)
    {
        printf("ERROR: Verificacion de CVV fallida.\n");
        presionar_para_continuar();
        return;
    }

    // Confirmación final
    printf("\n¿Esta seguro de anular la transaccion %s? (s/n): ", transaccion->numero_referencia);
    if (!fgets(opcion, sizeof(opcion), stdin))
    {
        printf("Error al leer opcion.\n");
        presionar_para_continuar();
        return;
    }
    eliminar_salto_linea(opcion);

    if (opcion[0] == 's' || opcion[0] == 'S')
    {
        transaccion->tipo = ANULACION;
        printf("Transaccion ANULADA exitosamente.\n");
    }
    else
    {
        printf("Anulacion cancelada.\n");
    }

    presionar_para_continuar();
}