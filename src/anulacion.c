#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "anulacion.h"
#include "utilidades.h"
#include "transacciones.h"
#include "validaciones.h"

// Función auxiliar para validar formato de referencia
int validar_formato_referencia(const char *referencia)
{
    // Debe empezar con "REF" seguido de 6 dígitos
    if (strlen(referencia) != REF_LENGTH - 1)
    {
        return 0;
    }

    // Verificar que empiece con "REF"
    if (strncmp(referencia, "REF", 3) != 0)
    {
        return 0;
    }

    // Verificar que los últimos 6 caracteres sean dígitos
    for (int i = 3; i < 9; i++)
    {
        if (!isdigit((unsigned char)referencia[i]))
        {
            return 0;
        }
    }

    return 1;
}

void ejecutar_anulacion(SistemaFinanciero *sistema)
{
    if (sistema->cantidad == 0)
    {
        printf("=== ANULACION DE COMPRA ===\n\n");
        printf("No hay transacciones para anular.\n");
        presionar_para_continuar();
        return;
    }

    char referencia[REF_LENGTH];
    char pan_verificacion[20];
    char cvv_verificacion[5];
    char input_buffer[50];

    printf("=== ANULACION DE COMPRA ===\n\n");
    printf("Ingrese 'q' en cualquier momento para cancelar\n\n");

    // REFERENCIA con validación mejorada
    do
    {
        printf("Numero de referencia (formato REF000001): ");
        if (!fgets(input_buffer, sizeof(input_buffer), stdin))
        {
            printf("Error al leer referencia.\n");
            presionar_para_continuar();
            return;
        }
        eliminar_salto_linea(input_buffer);

        if (strcmp(input_buffer, "q") == 0 || strcmp(input_buffer, "Q") == 0)
        {
            printf("Anulacion cancelada por el usuario.\n");
            presionar_para_continuar();
            return;
        }

        if (!validar_formato_referencia(input_buffer))
        {
            printf("ERROR: Formato de referencia invalido. Debe ser REF seguido de 6 digitos (ej: REF000001).\n");
            continue;
        }

        strncpy(referencia, input_buffer, sizeof(referencia) - 1);
        referencia[sizeof(referencia) - 1] = '\0';
        break;

    } while (1);

    Transaccion *transaccion = buscar_transaccion(sistema, referencia);

    if (!transaccion)
    {
        printf("ERROR: Numero de referencia no existe.\n");

        printf("\nReferencias existentes:\n");
        for (int i = 0; i < sistema->cantidad; i++)
        {
            if (sistema->transacciones[i].tipo == COMPRA)
            {
                printf("- %s\n", sistema->transacciones[i].numero_referencia);
            }
        }
        printf("\n");
        presionar_para_continuar();
        return;
    }

    if (transaccion->tipo == ANULACION)
    {
        printf("ERROR: La transaccion %s ya esta ANULADA.\n", transaccion->numero_referencia);
        presionar_para_continuar();
        return;
    }

    // Mostrar información de la transacción
    char monto_str[20];
    char pan_enmascarado[20];

    formatear_monto(transaccion->monto, monto_str, sizeof(monto_str));
    enmascarar_pan(transaccion->pan, pan_enmascarado);

    printf("\n=== INFORMACION DE LA TRANSACCION ===\n");
    printf("Referencia: %s\n", transaccion->numero_referencia);
    printf("Monto: %s\n", monto_str);
    printf("PAN: %s\n", pan_enmascarado);
    printf("Franquicia: %s\n", transaccion->franquicia);
    printf("=====================================\n\n");

    // OBTENER los últimos 4 dígitos reales del PAN de la transacción
    int pan_longitud = strlen(transaccion->pan);
    char ultimos_cuatro_real[5];
    strncpy(ultimos_cuatro_real, transaccion->pan + pan_longitud - 4, 4);
    ultimos_cuatro_real[4] = '\0';

    // VERIFICACIÓN DE PAN (últimos 4 dígitos) - VALIDAR INMEDIATAMENTE
    do
    {
        printf("Ultimos 4 digitos del PAN: ");
        if (!fgets(input_buffer, sizeof(input_buffer), stdin))
        {
            printf("Error al leer PAN.\n");
            presionar_para_continuar();
            return;
        }
        eliminar_salto_linea(input_buffer);

        if (strcmp(input_buffer, "q") == 0 || strcmp(input_buffer, "Q") == 0)
        {
            printf("Anulacion cancelada por el usuario.\n");
            presionar_para_continuar();
            return;
        }

        // Validar longitud exacta de 4 dígitos
        int longitud = strlen(input_buffer);
        if (longitud != 4)
        {
            printf("ERROR: Debe ingresar exactamente 4 digitos.\n");
            continue;
        }

        // Validar que sean solo dígitos
        int es_valido = 1;
        for (int i = 0; i < longitud; i++)
        {
            if (!isdigit((unsigned char)input_buffer[i]))
            {
                es_valido = 0;
                break;
            }
        }

        if (!es_valido)
        {
            printf("ERROR: Solo se permiten digitos numericos.\n");
            continue;
        }

        //  VALIDAR INMEDIATAMENTE si coincide con los últimos 4 dígitos reales
        if (strcmp(ultimos_cuatro_real, input_buffer) != 0)
        {
            printf("ERROR: Los ultimos 4 digitos no coinciden. Intente nuevamente.\n");
            continue; // NO pasa al CVV, vuelve a pedir el PAN
        }

        // Si llegó aquí, el PAN es correcto
        strncpy(pan_verificacion, input_buffer, sizeof(pan_verificacion) - 1);
        pan_verificacion[sizeof(pan_verificacion) - 1] = '\0';
        break;

    } while (1);

    // VERIFICACIÓN DE CVV - SOLO SE EJECUTA SI EL PAN FUE CORRECTO
    do
    {
        printf("CVV: ");
        if (!fgets(input_buffer, sizeof(input_buffer), stdin))
        {
            printf("Error al leer CVV.\n");
            presionar_para_continuar();
            return;
        }
        eliminar_salto_linea(input_buffer);

        if (strcmp(input_buffer, "q") == 0 || strcmp(input_buffer, "Q") == 0)
        {
            printf("Anulacion cancelada por el usuario.\n");
            presionar_para_continuar();
            return;
        }

        // USAR LA FUNCIÓN EXISTENTE validar_cvv_segun_franquicia
        if (!validar_cvv_segun_franquicia(input_buffer, transaccion->franquicia))
        {
            if (strcmp(transaccion->franquicia, "American Express") == 0)
            {
                printf("ERROR: American Express requiere 4 digitos para CVV.\n");
            }
            else if (strcmp(transaccion->franquicia, "Desconocida") != 0)
            {
                printf("ERROR: %s requiere 3 digitos para CVV.\n", transaccion->franquicia);
            }
            else
            {
                printf("ERROR: CVV debe tener 3 o 4 digitos.\n");
            }
            continue;
        }

        // VALIDAR INMEDIATAMENTE si coincide con el CVV real
        if (strcmp(transaccion->cvv, input_buffer) != 0)
        {
            printf("ERROR: El CVV no coincide. Intente nuevamente.\n");
            continue; // Vuelve a pedir el CVV
        }

        strncpy(cvv_verificacion, input_buffer, sizeof(cvv_verificacion) - 1);
        cvv_verificacion[sizeof(cvv_verificacion) - 1] = '\0';
        break;

    } while (1);

    // Confirmación final - SOLO LLEGA AQUÍ SI AMBAS VALIDACIONES FUERON EXITOSAS
    char confirmacion = validar_confirmacion_sn("Esta seguro de anular la transaccion?");

    if (confirmacion == 's')
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