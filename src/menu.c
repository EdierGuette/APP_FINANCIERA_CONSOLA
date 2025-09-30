#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "utilidades.h"
#include "compra.h"
#include "anulacion.h"
#include "cierre.h"
#include "reimpresion.h"
#include "reporte.h"

void mostrar_menu_principal()
{
    printf("=== SISTEMA FINANCIERO ===\n\n");
    printf("1. Compra\n");
    printf("2. Anulacion\n");
    printf("3. Cierre\n");
    printf("4. Reimpresion\n");
    printf("5. Reporte de Totales\n");
    printf("6. Salir\n\n");
    printf("Seleccione una opcion: ");
}

void ejecutar_menu_principal(SistemaFinanciero *sistema)
{
    int opcion;
    char buffer[10];

    do
    {
        limpiar_consola();
        mostrar_menu_principal();

        if (fgets(buffer, sizeof(buffer), stdin))
        {
            eliminar_salto_linea(buffer);
            opcion = atoi(buffer);

            limpiar_consola();

            switch (opcion)
            {
            case 1:
                ejecutar_compra(sistema);
                break;
            case 2:
                ejecutar_anulacion(sistema);
                break;
            case 3:
                ejecutar_cierre(sistema);
                break;
            case 4:
                ejecutar_reimpresion(sistema);
                break;
            case 5:
                ejecutar_reporte(sistema);
                break;
            case 6:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                presionar_para_continuar();
                break;
            }
        }
    } while (opcion != 6);
}