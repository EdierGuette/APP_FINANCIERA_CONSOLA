#ifndef ANULACION_H
#define ANULACION_H

#include "transacciones.h"

void ejecutar_anulacion(SistemaFinanciero *sistema);
int validar_formato_referencia(const char *referencia);
#endif