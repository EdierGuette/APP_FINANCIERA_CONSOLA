#include <stdio.h>
#include <string.h>
#include "transacciones.h"

void inicializar_sistema(SistemaFinanciero *sistema)
{
    sistema->cantidad = 0;
    sistema->siguiente_referencia = 1;
    memset(sistema->transacciones, 0, sizeof(sistema->transacciones));
}

int agregar_transaccion(SistemaFinanciero *sistema, Transaccion transaccion)
{
    if (sistema->cantidad >= MAX_TRANSACCIONES)
    {
        return 0;
    }

    sistema->transacciones[sistema->cantidad] = transaccion;
    sistema->cantidad++;
    return 1;
}
 
Transaccion *buscar_transaccion(SistemaFinanciero *sistema, const char *referencia)
{
    for (int i = 0; i < sistema->cantidad; i++)
    {
        if (strcmp(sistema->transacciones[i].numero_referencia, referencia) == 0)
        {
            return &sistema->transacciones[i];
        }
    }
    return NULL;
}