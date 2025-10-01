#ifndef TRANSACCIONES_H
#define TRANSACCIONES_H

#define MAX_TRANSACCIONES 20
#define PAN_LENGTH 16
#define REF_LENGTH 10
#define FECHA_LENGTH 6

typedef enum
{
    COMPRA,   // Transacción vigente
    ANULACION // Transacción cancelada (no vigente)
} TipoTransaccion;

typedef struct
{
    char numero_referencia[REF_LENGTH];
    long monto;
    char pan[PAN_LENGTH + 1];
    char cvv[5];
    char fecha_expiracion[FECHA_LENGTH];
    TipoTransaccion tipo;
    char franquicia[20];
    char fecha_hora[20];
} Transaccion;

typedef struct
{
    Transaccion transacciones[MAX_TRANSACCIONES];
    int cantidad;
    int siguiente_referencia;
} SistemaFinanciero;

void inicializar_sistema(SistemaFinanciero *sistema);
int agregar_transaccion(SistemaFinanciero *sistema, Transaccion transaccion);
Transaccion *buscar_transaccion(SistemaFinanciero *sistema, const char *referencia);

#endif 