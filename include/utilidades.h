#ifndef UTILIDADES_H
#define UTILIDADES_H

void limpiar_consola();
void presionar_para_continuar();
void eliminar_salto_linea(char *cadena);
long convertir_monto_a_centavos(const char *monto);
void formatear_monto(long centavos, char *buffer, int buffer_size);
void enmascarar_pan(const char *pan, char *pan_enmascarado);
void limpiar_buffer_entrada();
void eliminar_salto_linea(char *cadena);

#endif