#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "validaciones.h"

int validar_monto(const char *monto)
{
    int longitud = strlen(monto);
    int punto_encontrado = 0;
    int digitos_despues_punto = 0;
    int digitos_antes_punto = 0;

    if (longitud == 0 || longitud > 12)
    {
        return 0;
    }

    // Verificar que el primer carácter sea un dígito (no punto)
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
        // Aquí se valida que NO se permitan espacios u otros caracteres
        else if (!isdigit((unsigned char)monto[i]))
        {
            return 0; // Si no es dígito → inválido (espacios, letras, símbolos, etc.)
        }
        else if (punto_encontrado)
        {
            digitos_despues_punto++;
            if (digitos_despues_punto > 2)
                return 0; // Máximo 2 decimales
        }
        else
        {
            digitos_antes_punto++;
            if (digitos_antes_punto > 12)
                return 0; // Máximo 12 dígitos antes del punto
        }
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

    // CVV debe tener exactamente 3 o 4 dígitos
    if (longitud != 3 && longitud != 4)
    {
        return 0;
    }

    // Validar que todos los caracteres sean dígitos (nada de espacios ni caracteres especiales)
    for (int i = 0; i < longitud; i++)
    {
        if (!isdigit((unsigned char)cvv[i]))
        {
            return 0;
        }
    }

    return 1; // válido
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
    // Validar que PAN no sea NULL ni vacío
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
            strcpy(franquicia, "Desconocida"); // Si hay caracteres raros, se rechaza
            return;
        }
    }

    // Asegurarse de que tenga al menos 2 dígitos para evitar acceso inválido
    int longitud = strlen(pan);
    int primer_digito = pan[0] - '0';
    int primeros_dos = (longitud >= 2) ? ((pan[0] - '0') * 10 + (pan[1] - '0')) : -1;

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
    else if (primeros_dos >= 60 && primeros_dos <= 65)
    {
        strcpy(franquicia, "Discover");
    }
    else if (primer_digito == 3)
    {
        strcpy(franquicia, "Diners Club");
    }
    else if (primer_digito == 6)
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
    // 🔹 Validar que ningún campo esté vacío o nulo
    if (!pan || !cvv || !fecha || !franquicia)
    {
        return 0;
    }
    if (strlen(pan) == 0 || strlen(cvv) == 0 || strlen(fecha) == 0)
    {
        return 0;
    }

    // 🔹 Validar PAN (número de tarjeta)
    if (!validar_pan(pan))
    {
        return 0;
    }

    // 🔹 Validar CVV
    if (!validar_cvv(cvv))
    {
        return 0;
    }

    // 🔹 Validar fecha de expiración
    if (!validar_fecha_expiracion(fecha))
    {
        return 0;
    }

    // 🔹 Identificar franquicia de la tarjeta
    identificar_franquicia(pan, franquicia);

    // 🔹 Rechazar franquicia desconocida
    if (strcmp(franquicia, "Desconocida") == 0)
    {
        return 0;
    }

    return 1; // ✅ Todo válido
}