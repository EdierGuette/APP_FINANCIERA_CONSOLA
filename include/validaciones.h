#ifndef VALIDACIONES_H
#define VALIDACIONES_H

int validar_monto(const char *monto);
int algoritmo_luhn(const char *pan);
int validar_pan(const char *pan);
int validar_cvv(const char *cvv);
int validar_fecha_expiracion(const char *fecha);
void identificar_franquicia(const char *pan, char *franquicia);
int validar_tarjeta_completa(const char *pan, const char *cvv, const char *fecha, char *franquicia);
int validar_cvv_segun_franquicia(const char *cvv, const char *franquicia);

#endif