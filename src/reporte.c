#include <stdio.h>
#include <string.h>
#include "reporte.h"
#include "utilidades.h"
#include "transacciones.h"

void ejecutar_reporte(SistemaFinanciero *sistema)
{
    int compras_vigentes = 0, compras_anuladas = 0;
    long total_compras_vigentes = 0, total_compras_anuladas = 0;
    char monto_str[20];

    // CONTAR SOLO COMPRAS VIGENTES (no anuladas)
    for (int i = 0; i < sistema->cantidad; i++)
    {
        if (sistema->transacciones[i].tipo == COMPRA)
        {
            compras_vigentes++;
            total_compras_vigentes += sistema->transacciones[i].monto;
        }
        else
        {
            compras_anuladas++;
            total_compras_anuladas += sistema->transacciones[i].monto;
        }
    }

    printf("=== REPORTE DE TOTALES ===\n\n");
    printf("Total transacciones: %d\n", sistema->cantidad);
    printf("Compras vigentes: %d\n", compras_vigentes);
    printf("Compras anuladas: %d\n\n", compras_anuladas);

    // MOSTRAR MONTOS SEPARADOS
    formatear_monto(total_compras_vigentes, monto_str, sizeof(monto_str));
    printf("Total compras vigentes: %s\n", monto_str);

    formatear_monto(total_compras_anuladas, monto_str, sizeof(monto_str));
    printf("Total compras anuladas: %s\n", monto_str);

    // NETO = SOLO COMPRAS VIGENTES (NUNCA negativo)
    formatear_monto(total_compras_vigentes, monto_str, sizeof(monto_str));
    printf("Neto (compras vigentes): %s\n", monto_str);

    presionar_para_continuar();
}