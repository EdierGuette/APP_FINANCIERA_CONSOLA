#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "transacciones.h"
#include "archivos.h"

int main()
{
    SistemaFinanciero sistema;

    inicializar_sistema(&sistema);

    if (!cargar_transacciones(&sistema))
    {
        printf("Error al cargar transacciones. Iniciando con sistema vacio.\n");
    }

    ejecutar_menu_principal(&sistema);

    guardar_transacciones(&sistema);

    return 0;
} 