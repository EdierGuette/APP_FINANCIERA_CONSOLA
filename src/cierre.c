#include <stdio.h>
#include <string.h>
#include "cierre.h"
#include "utilidades.h"
#include "transacciones.h"
#include "validaciones.h"

void mostrar_resumen(SistemaFinanciero *sistema)
{
    int compras_vigentes = 0, compras_anuladas = 0;
    long total_compras_vigentes = 0, total_compras_anuladas = 0;
    char monto_str[20];

    // CONTAR SOLO COMPRAS VIGENTES
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

    printf("=== RESUMEN DE CIERRE ===\n\n");
    printf("Total transacciones: %d\n", sistema->cantidad);
    printf("Compras vigentes: %d\n", compras_vigentes);
    printf("Compras anuladas: %d\n", compras_anuladas);

    formatear_monto(total_compras_vigentes, monto_str, sizeof(monto_str));
    printf("Total compras vigentes: %s\n", monto_str);

    formatear_monto(total_compras_anuladas, monto_str, sizeof(monto_str));
    printf("Total compras anuladas: %s\n", monto_str);

    // NETO = SOLO COMPRAS VIGENTES
    formatear_monto(total_compras_vigentes, monto_str, sizeof(monto_str));
    printf("Neto (compras vigentes): %s\n\n", monto_str);
}

void ejecutar_cierre(SistemaFinanciero *sistema)
{
    if (sistema->cantidad == 0)
    {
        printf("=== RESUMEN DE CIERRE ===\n\n");
        printf("No hay transacciones para cerrar.\n");
        presionar_para_continuar();
        return;
    }

    mostrar_resumen(sistema);

    // Reutilizar la misma función de confirmación
    char confirmacion = validar_confirmacion_sn("Esta seguro de realizar el cierre?");

    if (confirmacion == 's')
    {
        sistema->cantidad = 0;
        sistema->siguiente_referencia = 1;
        printf("Cierre realizado exitosamente.\n");
    }
    else
    {
        printf("Cierre cancelado.\n");
    }

    presionar_para_continuar();
}