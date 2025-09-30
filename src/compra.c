#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "compra.h"
#include "validaciones.h"
#include "utilidades.h"
#include "transacciones.h"

void ejecutar_compra(SistemaFinanciero *sistema)
{
    if (sistema->cantidad >= MAX_TRANSACCIONES)
    {
        printf("Error: No se pueden agregar mas transacciones. Limite alcanzado.\n");
        presionar_para_continuar();
        return;
    }

    Transaccion nueva;
    nueva.tipo = COMPRA;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(nueva.fecha_hora, 20, "%d/%m/%Y %H:%M", tm_info);

    snprintf(nueva.numero_referencia, REF_LENGTH, "REF%06d", sistema->siguiente_referencia);

    if (capturar_datos_compra(&nueva) == 0)
    {
        // Usuario canceló la compra
        printf("Compra cancelada por el usuario.\n");
        presionar_para_continuar();
        return;
    }

    if (!validar_tarjeta_completa(nueva.pan, nueva.cvv, nueva.fecha_expiracion, nueva.franquicia))
    {
        printf("Error: Tarjeta no valida.\n");
        presionar_para_continuar();
        return;
    }

    if (agregar_transaccion(sistema, nueva))
    {
        sistema->siguiente_referencia++;
        printf("Compra realizada exitosamente. Referencia: %s\n", nueva.numero_referencia);
    }
    else
    {
        printf("Error: No se pudo realizar la compra.\n");
    }

    presionar_para_continuar();
}
int capturar_datos_compra(Transaccion *transaccion)
{
    char monto_str[20];
    char input_buffer[50]; // Buffer para fgets
    char franquicia_temp[20];

    printf("Ingrese 'q' en cualquier momento para cancelar\n\n");
    printf("=== NUEVA COMPRA ===\n\n");

    // MONTO usando fgets
    do
    {
        printf("Monto (max 12 digitos): $");

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
        {
            printf("Error al leer entrada. Desea cancelar? (s/n): ");
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
            {
                return 0;
            }
            eliminar_salto_linea(input_buffer);
            if (input_buffer[0] == 's' || input_buffer[0] == 'S')
            {
                return 0;
            }
            continue;
        }

        eliminar_salto_linea(input_buffer);

        if (strcmp(input_buffer, "q") == 0 || strcmp(input_buffer, "Q") == 0)
        {
            return 0; // Usuario canceló
        }

        // Copiar a monto_str para mantener compatibilidad
        strncpy(monto_str, input_buffer, sizeof(monto_str) - 1);
        monto_str[sizeof(monto_str) - 1] = '\0';

        if (validar_monto(monto_str))
        {
            // Convertir a centavos o a número
            long monto_centavos = convertir_monto_a_centavos(monto_str);

            // Validar que el monto sea mayor que 0
            if (monto_centavos <= 0)
            {
                printf("El monto debe ser mayor que 0. Intente nuevamente.\n");
                continue;
            }

            transaccion->monto = monto_centavos;
            break;
        }
        printf("Monto no valido. Intente nuevamente.\n");
    } while (1);

    // PAN usando fgets - CON VALIDACIÓN MEJORADA
    do
    {
        printf("Numero de tarjeta: ");

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
        {
            printf("Error al leer entrada. Desea cancelar? (s/n): ");
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
            {
                return 0;
            }
            eliminar_salto_linea(input_buffer);
            if (input_buffer[0] == 's' || input_buffer[0] == 'S')
            {
                return 0;
            }
            continue;
        }

        eliminar_salto_linea(input_buffer);

        if (strcmp(input_buffer, "q") == 0 || strcmp(input_buffer, "Q") == 0)
        {
            return 0;
        }

        // PRIMERO identificar la franquicia con el input completo
        identificar_franquicia(input_buffer, franquicia_temp);

        // LUEGO validar longitud específica según franquicia
        if (!validar_longitud_segun_franquicia(input_buffer, franquicia_temp))
        {
            printf("Numero de tarjeta no valido. Longitud incorrecta para %s.\n", franquicia_temp);
            continue;
        }

        // FINALMENTE validar el PAN (dígitos y algoritmo Luhn)
        if (!validar_pan(input_buffer))
        {
            printf("Numero de tarjeta no valido.\n");
            continue;
        }

        // Solo si todo es válido, copiar al struct
        strncpy(transaccion->pan, input_buffer, sizeof(transaccion->pan) - 1);
        transaccion->pan[sizeof(transaccion->pan) - 1] = '\0';
        strncpy(transaccion->franquicia, franquicia_temp, sizeof(transaccion->franquicia) - 1);
        transaccion->franquicia[sizeof(transaccion->franquicia) - 1] = '\0';

        printf("Tarjeta detectada: %s\n", transaccion->franquicia);
        break;

    } while (1);

    // CVV usando fgets - AHORA VALIDADO SEGÚN FRANQUICIA
    do
    {
        printf("CVV: ");

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
        {
            printf("Error al leer entrada. Desea cancelar? (s/n): ");
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
            {
                return 0;
            }
            eliminar_salto_linea(input_buffer);
            if (input_buffer[0] == 's' || input_buffer[0] == 'S')
            {
                return 0;
            }
            continue;
        }

        eliminar_salto_linea(input_buffer);

        if (strcmp(input_buffer, "q") == 0 || strcmp(input_buffer, "Q") == 0)
        {
            return 0;
        }

        // VALIDAR ANTES de copiar al struct
        if (!validar_cvv_segun_franquicia(input_buffer, transaccion->franquicia))
        {
            // Mensaje de error específico según la franquicia
            if (strcmp(transaccion->franquicia, "American Express") == 0)
            {
                printf("CVV no valido. American Express requiere exactamente 4 digitos.\n");
            }
            else if (strcmp(transaccion->franquicia, "Desconocida") != 0)
            {
                printf("CVV no valido. %s requiere exactamente 3 digitos.\n", transaccion->franquicia);
            }
            else
            {
                printf("CVV no valido. Debe tener exactamente 3 o 4 digitos.\n");
            }
            continue;
        }

        // Solo si es válido, copiar al struct
        strncpy(transaccion->cvv, input_buffer, sizeof(transaccion->cvv) - 1);
        transaccion->cvv[sizeof(transaccion->cvv) - 1] = '\0';
        break;

    } while (1);

    // FECHA usando fgets
    do
    {
        printf("Fecha expiracion (MM/YY): ");

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
        {
            printf("Error al leer entrada. Desea cancelar? (s/n): ");
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
            {
                return 0;
            }
            eliminar_salto_linea(input_buffer);
            if (input_buffer[0] == 's' || input_buffer[0] == 'S')
            {
                return 0;
            }
            continue;
        }

        eliminar_salto_linea(input_buffer);

        if (strcmp(input_buffer, "q") == 0 || strcmp(input_buffer, "Q") == 0)
        {
            return 0;
        }

        strncpy(transaccion->fecha_expiracion, input_buffer, sizeof(transaccion->fecha_expiracion) - 1);
        transaccion->fecha_expiracion[sizeof(transaccion->fecha_expiracion) - 1] = '\0';

        if (validar_fecha_expiracion(transaccion->fecha_expiracion))
        {
            break;
        }
        printf("Fecha no valida.\n");
    } while (1);

    printf("\n");
    return 1;
}