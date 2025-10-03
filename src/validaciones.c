#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include "validaciones.h"
#include "reimpresion.h"
#include "utilidades.h"

int validar_monto(const char *monto)
{
    int longitud = strlen(monto);
    int punto_encontrado = 0;
    int digitos_despues_punto = 0;
    int digitos_antes_punto = 0;
    int total_digitos = 0;

    if (longitud == 0 || longitud > 13) // Máximo: 12 dígitos o 10+1punto+2decimales
    {
        return 0;
    }

    // Verificar que el primer carácter sea un dígito
    if (!isdigit(monto[0]))
    {
        return 0;
    }

    for (int i = 0; i < longitud; i++)
    {
        if (monto[i] == '.')
        {
            if (punto_encontrado)
                return 0; // Más de un punto
            if (i == 0)
                return 0; // Punto al inicio
            if (i == longitud - 1)
                return 0; // Punto al final
            punto_encontrado = 1;
        }
        else if (!isdigit((unsigned char)monto[i]))
        {
            return 0; // Si no es dígito → inválido
        }
        else
        {
            total_digitos++;
            if (punto_encontrado)
            {
                digitos_despues_punto++;
            }
            else
            {
                digitos_antes_punto++;
            }
        }
    }

    // Validar máximo 12 dígitos en total
    if (total_digitos > 12)
    {
        return 0;
    }

    // Si hay punto, debe tener exactamente 2 decimales
    if (punto_encontrado && digitos_despues_punto != 2)
    {
        return 0;
    }

    // Si hay punto, máximo 10 dígitos enteros (10 + 2 = 12 total)
    if (punto_encontrado && digitos_antes_punto > 10)
    {
        return 0;
    }

    // VALIDACIÓN NUEVA: Rechazar ceros 
    int todos_ceros = 1;
    for (int i = 0; i < longitud; i++)
    {
        if (monto[i] != '.' && monto[i] != '0')
        {
            todos_ceros = 0;
            break;
        }
    }
    if (todos_ceros)
    {
        return 0;
    }


    return 1;
}

int algoritmo_luhn(const char *pan)
{
    int suma = 0;
    int longitud = strlen(pan);
    int doblar = 0;

    for (int i = longitud - 1; i >= 0; i--)
    {
        int digito = pan[i] - '0';

        if (doblar)
        {
            digito *= 2;
            if (digito > 9)
                digito -= 9;
        }

        suma += digito;
        doblar = !doblar;
    }

    return (suma % 10) == 0;
}

int validar_pan(const char *pan)
{
    int longitud = strlen(pan);

    // Longitud mínima y máxima para tarjetas (13 a 19)
    if (longitud < 13 || longitud > 19)
    {
        return 0;
    }

    // Validar que todos sean dígitos (sin espacios ni caracteres especiales)
    for (int i = 0; i < longitud; i++)
    {
        if (!isdigit((unsigned char)pan[i]))
        {
            return 0;
        }
    }

    // Validar usando algoritmo de Luhn
    return algoritmo_luhn(pan);
}

int validar_cvv(const char *cvv)
{
    int longitud = strlen(cvv);

    // Validar longitud básica - EXACTA, no mínimo/máximo
    if (longitud != 3 && longitud != 4)
    {
        return 0;
    }

    // Validar que todos los caracteres sean dígitos
    for (int i = 0; i < longitud; i++)
    {
        if (!isdigit((unsigned char)cvv[i]))
        {
            return 0;
        }
    }

    return 1;
}

int validar_fecha_expiracion(const char *fecha)
{
    // Debe tener exactamente 5 caracteres: "MM/YY"
    if (strlen(fecha) != 5)
        return 0;

    // El tercer carácter debe ser '/'
    if (fecha[2] != '/')
        return 0;

    // Validar que los demás sean dígitos
    if (!isdigit((unsigned char)fecha[0]) ||
        !isdigit((unsigned char)fecha[1]) ||
        !isdigit((unsigned char)fecha[3]) ||
        !isdigit((unsigned char)fecha[4]))
    {
        return 0;
    }

    // Convertir a números
    int mes = (fecha[0] - '0') * 10 + (fecha[1] - '0');
    int ano_2digitos = (fecha[3] - '0') * 10 + (fecha[4] - '0');

    // Validar mes (1-12)
    if (mes < 1 || mes > 12)
        return 0;

    // Rechazar fechas absurdas como 00/00
    if (mes == 0 || ano_2digitos == 0)
        return 0;

    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    int ano_actual_completo = tm_info->tm_year + 1900;
    int mes_actual = tm_info->tm_mon + 1;

    // Calcular año completo (manejo del siglo)
    int siglo_base = (ano_actual_completo / 100) * 100;
    int ano_completo = siglo_base + ano_2digitos;

    // Si el año de 2 dígitos es menor que el año actual de 2 dígitos,
    // asumimos que es del siguiente siglo
    if (ano_2digitos < (ano_actual_completo % 100))
    {
        ano_completo += 100;
    }

    // Validar que no sea mayor a 10 años en el futuro
    int ano_limite = ano_actual_completo + 10;
    if (ano_completo > ano_limite)
        return 0;

    // Si es el año actual, validar que el mes no esté vencido
    if (ano_completo == ano_actual_completo && mes < mes_actual)
        return 0;

    return 1; // Fecha válida
}

void identificar_franquicia(const char *pan, char *franquicia)
{
    if (pan == NULL || strlen(pan) < 1)
    {
        strcpy(franquicia, "Desconocida");
        return;
    }

    // Validar que el PAN contenga solo dígitos
    for (int i = 0; pan[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)pan[i]))
        {
            strcpy(franquicia, "Desconocida");
            return;
        }
    }

    int longitud = strlen(pan);
    int primer_digito = pan[0] - '0';
    int primeros_dos = (longitud >= 2) ? ((pan[0] - '0') * 10 + (pan[1] - '0')) : -1;
    int primeros_tres = (longitud >= 3) ? (primeros_dos * 10 + (pan[2] - '0')) : -1;
    int primeros_cuatro = (longitud >= 4) ? (primeros_tres * 10 + (pan[3] - '0')) : -1;

    // Heurística mejorada y más precisa
    if (primer_digito == 4)
    {
        strcpy(franquicia, "Visa");
    }
    else if (primer_digito == 5 && primeros_dos >= 51 && primeros_dos <= 55)
    {
        strcpy(franquicia, "Mastercard");
    }
    else if (primeros_dos == 34 || primeros_dos == 37)
    {
        strcpy(franquicia, "American Express");
    }
    else if (primeros_dos == 35)
    {
        strcpy(franquicia, "JCB");
    }
    else if (primeros_dos == 36 || primeros_dos == 38 || primeros_dos == 39 ||
             (primeros_tres >= 300 && primeros_tres <= 305))
    {
        strcpy(franquicia, "Diners Club");
    }
    else if (primer_digito == 6 && (primeros_cuatro == 6011 || primeros_dos == 65 ||
                                    (primeros_tres >= 644 && primeros_tres <= 649)))
    {
        strcpy(franquicia, "Discover");
    }
    else
    {
        strcpy(franquicia, "Desconocida");
    }
}

int validar_tarjeta_completa(const char *pan, const char *cvv, const char *fecha, char *franquicia)
{
    //  Validar que ningún campo esté vacío o nulo
    if (!pan || !cvv || !fecha || !franquicia)
    {
        return 0;
    }
    if (strlen(pan) == 0 || strlen(cvv) == 0 || strlen(fecha) == 0)
    {
        return 0;
    }

    //  Validar PAN (número de tarjeta)
    if (!validar_pan(pan))
    {
        return 0;
    }

    //  Validar CVV
    if (!validar_cvv(cvv))
    {
        return 0;
    }

    //  Validar fecha de expiración
    if (!validar_fecha_expiracion(fecha))
    {
        return 0;
    }

    //  Identificar franquicia de la tarjeta
    identificar_franquicia(pan, franquicia);

    //  Rechazar franquicia desconocida
    if (strcmp(franquicia, "Desconocida") == 0)
    {
        return 0;
    }

    return 1; //  Todo válido
}
int validar_cvv_segun_franquicia(const char *cvv, const char *franquicia)
{
    int longitud = strlen(cvv);

    // Validar que todos los caracteres sean dígitos
    for (int i = 0; i < longitud; i++)
    {
        if (!isdigit((unsigned char)cvv[i]))
        {
            return 0;
        }
    }

    // Validar longitud EXACTA según franquicia
    if (strcmp(franquicia, "American Express") == 0)
    {
        // American Express usa CVV de 4 dígitos EXACTOS
        if (longitud != 4)
        {
            return 0;
        }
    }
    else if (strcmp(franquicia, "Desconocida") != 0)
    {
        // Otras franquicias (Visa, Mastercard, etc.) usan CVV de 3 dígitos EXACTOS
        if (longitud != 3)
        {
            return 0;
        }
    }
    else
    {
        // Para franquicia desconocida, aceptar SOLO 3 o 4 dígitos
        if (longitud != 3 && longitud != 4)
        {
            return 0;
        }
    }

    return 1; // válido
}
int validar_longitud_segun_franquicia(const char *pan, const char *franquicia)
{
    int longitud = strlen(pan);

    if (strcmp(franquicia, "Visa") == 0)
    {
        // Visa: 13, 16, 19 dígitos
        return (longitud == 13 || longitud == 16 || longitud == 19);
    }
    else if (strcmp(franquicia, "Mastercard") == 0)
    {
        // Mastercard: 16 dígitos
        return (longitud == 16);
    }
    else if (strcmp(franquicia, "American Express") == 0)
    {
        // American Express: 15 dígitos
        return (longitud == 15);
    }
    else if (strcmp(franquicia, "Discover") == 0)
    {
        // Discover: 16, 19 dígitos
        return (longitud == 16 || longitud == 19);
    }
    else if (strcmp(franquicia, "Diners Club") == 0)
    {
        // Diners Club: 14, 16, 19 dígitos
        return (longitud == 14 || longitud == 16 || longitud == 19);
    }
    else if (strcmp(franquicia, "JCB") == 0)
    {
        // JCB: 16, 17, 18, 19 dígitos
        return (longitud >= 16 && longitud <= 19);
    }

    // Para franquicia desconocida, aceptar el rango general 13-19
    return (longitud >= 13 && longitud <= 19);
}

// Función auxiliar para validar confirmación s/n
int validar_confirmacion_sn(const char *mensaje)
{
    char input_buffer[50];
    char opcion;

    do
    {
        printf("%s (s/n): ", mensaje);
        if (!fgets(input_buffer, sizeof(input_buffer), stdin))
        {
            printf("Error al leer opcion.\n");
            continue;
        }
        eliminar_salto_linea(input_buffer);

        // Validar que sea exactamente 1 carácter
        if (strlen(input_buffer) != 1)
        {
            printf("Opcion no valida. Ingrese solamente 's' para si o 'n' para no.\n");
            continue;
        }

        // Convertir a minúscula
        opcion = tolower((unsigned char)input_buffer[0]);

        if (opcion == 's' || opcion == 'n')
        {
            return opcion; // Retorna 's' o 'n'
        }
        else
        {
            printf("Opcion no valida. Ingrese solamente 's' para si o 'n' para no.\n");
        }
    } while (1);
}

// Función para validar navegación en reimpresión
char validar_navegacion_reimpresion(int indice_actual, int total_transacciones)
{
    char input_buffer[50];
    char opcion;

    do
    {
        printf("Seleccione: ");
        if (!fgets(input_buffer, sizeof(input_buffer), stdin))
        {
            printf("Error al leer opcion.\n");
            continue;
        }
        eliminar_salto_linea(input_buffer);

        // Validar que sea exactamente 1 carácter
        if (strlen(input_buffer) != 1)
        {
            printf("Opcion no valida. ");
            mostrar_opciones_navegacion(indice_actual, total_transacciones);
            continue;
        }

        // Convertir a minúscula
        opcion = tolower((unsigned char)input_buffer[0]);

        // Validar opciones permitidas con mensajes específicos para límites
        if (opcion == 'a')
        {
            if (indice_actual > 0)
            {
                return opcion;
            }
            else
            {
                printf("No hay mas transacciones hacia atras. ");
                mostrar_opciones_navegacion(indice_actual, total_transacciones);
            }
        }
        else if (opcion == 'd')
        {
            if (indice_actual < total_transacciones - 1)
            {
                return opcion;
            }
            else
            {
                printf("No hay mas transacciones hacia adelante. ");
                mostrar_opciones_navegacion(indice_actual, total_transacciones);
            }
        }
        else if (opcion == 'q')
        {
            return opcion;
        }
        else
        {
            printf("Opcion no valida. ");
            mostrar_opciones_navegacion(indice_actual, total_transacciones);
        }
    } while (1);
}