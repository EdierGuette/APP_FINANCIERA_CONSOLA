#ifndef COMPRA_H
#define COMPRA_H

#include "transacciones.h"

void ejecutar_compra(SistemaFinanciero *sistema);
int capturar_datos_compra(Transaccion *transaccion); 

#endif