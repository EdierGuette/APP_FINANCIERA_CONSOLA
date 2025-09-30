#include <stdio.h>
#include <stdlib.h>
#include "archivos.h"
#include "transacciones.h"

#define ARCHIVO_TRANSACCIONES "data/transacciones.dat"

int guardar_transacciones(SistemaFinanciero *sistema)
{
    FILE *archivo = fopen(ARCHIVO_TRANSACCIONES, "wb");
    if (!archivo)
    {
        printf("Error: No se pudo crear el archivo de transacciones.\n");
        return 0;
    }

    // Guardar metadata
    if (fwrite(&sistema->cantidad, sizeof(int), 1, archivo) != 1)
    {
        fclose(archivo);
        return 0;
    }
    if (fwrite(&sistema->siguiente_referencia, sizeof(int), 1, archivo) != 1)
    {
        fclose(archivo);
        return 0;
    }

    // Guardar transacciones
    if (sistema->cantidad > 0)
    {
        if (fwrite(sistema->transacciones, sizeof(Transaccion), sistema->cantidad, archivo) != sistema->cantidad)
        {
            fclose(archivo);
            return 0;
        }
    }

    fclose(archivo);
    return 1;
}

int cargar_transacciones(SistemaFinanciero *sistema)
{
    FILE *archivo = fopen(ARCHIVO_TRANSACCIONES, "rb");
    if (!archivo)
    {
        // Archivo no existe, empezar con sistema vacío
        return 0;
    }

    // Cargar metadata
    if (fread(&sistema->cantidad, sizeof(int), 1, archivo) != 1)
    {
        fclose(archivo);
        return 0;
    }
    if (fread(&sistema->siguiente_referencia, sizeof(int), 1, archivo) != 1)
    {
        fclose(archivo);
        return 0;
    }

    // Cargar transacciones
    if (sistema->cantidad > 0)
    {
        if (fread(sistema->transacciones, sizeof(Transaccion), sistema->cantidad, archivo) != sistema->cantidad)
        {
            fclose(archivo);
            return 0;
        }
    }

    fclose(archivo);
    return 1;
}