# 🏦 Sistema Financiero - Terminal POS  

Un sistema de **terminal punto de venta (POS)** desarrollado en **C** para procesamiento de transacciones financieras con validaciones robustas y gestión completa de operaciones.  

---

## 📋 Características  

### 💳 Procesamiento de Transacciones  
- **Compras:** Captura y validación completa de transacciones  
- **Anulaciones:** Reversión segura de transacciones con verificación  
- **Reimpresiones:** Recuperación y visualización de transacciones anteriores  
- **Cierres:** Generación de reportes financieros diarios  

### 🔒 Validaciones de Seguridad  
- **Algoritmo de Luhn:** Validación matemática de números de tarjeta  
- **Detección de Franquicias:** Identificación automática (Visa, Mastercard, Amex, etc.)  
- **Validación de CVV:** Verificación según tipo de tarjeta (3-4 dígitos)  
- **Fechas de Expiración:** Control de vigencia de tarjetas  
- **Formatos estrictos:** Prevención de inyección de datos  

### 💰 Gestión Financiera  
- Validación de montos con decimales  
- Conversión a centavos para precisión  
- Enmascaramiento de datos sensibles  
- Reportes detallados con estadísticas  

---

## 🏗️ Estructura del Proyecto  

```text
financiera/
├── src/
│   ├── main.c                 # Punto de entrada principal
│   ├── compras.c              # Gestión de transacciones de compra
│   ├── anulaciones.c          # Procesamiento de anulaciones
│   ├── cierres.c              # Generación de cierres diarios
│   ├── reimpresiones.c        # Reimpresión de transacciones
│   ├── reportes.c             # Reportes financieros
│   ├── transacciones.c        # Estructuras y gestión de datos
│   ├── validaciones.c         # Validaciones de tarjetas y montos
│   └── utilidades.c           # Funciones auxiliares
├── include/
│   ├── compras.h
│   ├── anulaciones.h
│   ├── cierres.h
│   ├── reimpresiones.h
│   ├── reportes.h
│   ├── transacciones.h
│   ├── validaciones.h
│   └── utilidades.h
├── build/                     # Archivos objeto (generados)
├── data/                      # Datos de transacciones
└── Makefile                   # Sistema de build
```
## 🚀 Instalación y Compilación  
### 📦 Prerrequisitos  
- **GCC** (GNU Compiler Collection)  
- **Sistema Unix/Linux o Windows** con entorno compatible  

### ⚙️ Compilación  
```bash
# Compilar y ejecutar directamente
make run

# Compilar el proyecto
make all

# Crear estructura de directorios necesaria
make install
```

### 🧹 Limpieza
```bash
# Limpiar archivos compilados
make clean

# Resetear base de datos de transacciones
make reset
```

# 💳 Tarjetas de Prueba para Sistema POS (Datos de Test)

> **IMPORTANTE:** Estos números son **solo para pruebas en entornos de desarrollo** (modo sandbox). **No** deben usarse en pasarelas de pago reales ni para transacciones en producción. Siempre cumple con las políticas y normas legales (PCI-DSS) y usa entornos de prueba proporcionados por tu PSP/servicio de pago. Estos ejemplos están pensados para validar validaciones (longitud, Luhn, CVV, formato, etc.) en tu terminal POS.

---

## 🧾 Resumen de Tarjetas de Prueba

| Franquicia | Número (Test)           | CVV       | Fecha (MM/YY) | Longitud |
|------------|-------------------------|-----------|---------------|----------|
| **VISA**   | `4111 1111 1111 1111`   | `123`     | `12/28`       | 16       |
| **Mastercard (51-55)** | `5555 5555 5555 4444` | `123` | `12/28` | 16 |
| **Mastercard (2221-2720)** | `2223 0031 2200 3222` | `123` | `12/28` | 16 |
| **American Express** | `3782 822463 10005` | `1234` (4 dígitos) | `12/28` | 15 |
| **Discover** | `6011 1111 1111 1117` | `123` | `12/28` | 16 |
| **Diners Club** | `3056 9309 0259 04` | `123` | `12/28` | 14 |
| **JCB** | `3530 1113 3330 0000` | `123` | `12/28` | 16 |
| **Tarjeta inválida (rechazo)** | `4111 1111 1111 1112` | `123` | `12/28` | — |

---

## 🧪 Datos de prueba para montos
Usa estos ejemplos para validar la lógica de montos y restricciones:

- Monto válido 1: `100.50`  
- Monto válido 2: `50.00`  
- Monto inválido (cero): `0`  
- Monto inválido (negativo): `-10`

---

## ⚠️ Puntos clave para pruebas y validaciones

- **Solo American Express** usa **CVV de 4 dígitos**; el resto usa 3 dígitos.  
- **Visa** puede aparecer con longitudes **13, 16 o 19** — en estos tests se usa 16.  
- **Mastercard** tiene dos rangos válidos: `51–55` y el rango más nuevo `2221–2720`.  
- Todos los números listados (excepto la tarjeta inválida) **cumplen el algoritmo de Luhn**; la tarjeta marcada como inválida cambia el último dígito para provocar fallo en Luhn.  
- Las fechas usadas son ejemplos; además debes validar que la fecha no esté vencida (comparar con la fecha del sistema).  
- Valida formato de **PAN** (solo dígitos, sin espacios en lógica interna) y **CVV** según franquicia.  
- Convierte montos a **centavos** internamente para evitar errores de punto flotante (ej. $100.50 → `10050` centavos).  
- En interfaces y logs **enmascara** el PAN (ej. `XXXX XXXX XXXX 1111`) y **no** almacenes CVV en almacenamiento persistente (cumplir PCI-DSS).

---

## 🔒 Recomendaciones de seguridad y cumplimiento

1. **Entorno de pruebas**: Ejecuta estas tarjetas únicamente contra entornos sandbox de tu proveedor de pagos (ej. Stripe, PayPal Sandbox, etc.).  
2. **No usar en producción**: Nunca intentes procesarlas por redes reales. Las pasarelas reales las rechazarán o las distinguirán como test.  
3. **Almacenamiento seguro**: No almacenes CVV ni datos sensibles en texto plano. En producción, sigue PCI-DSS: tokenización, cifrado y mínimos privilegios.  
4. **Logs y debugging**: En logs de desarrollo puedes mostrar PAN enmascarado; evita dejar datos sensibles en logs persistentes.  
5. **Pruebas automatizadas**: Incluye estos casos en tus suites de pruebas unitarias e integración para validar escenarios: aprobación, rechazo (Luhn fail), CVV incorrecto, fecha vencida, montos inválidos, duplicados.

---

## ✅ Casos de prueba sugeridos (ejemplos)

- **Compra válida (Visa)**: PAN `4111111111111111`, CVV `123`, monto `100.50` → esperado: *aprobado (modo test)*.  
- **Compra válida (Amex)**: PAN `378282246310005`, CVV `1234`, monto `50` → esperado: *aprobado (modo test)*.  
- **Rechazo Luhn**: PAN `4111111111111112`, CVV `123` → esperado: *rechazado por Luhn*.  
- **CVV inválido**: PAN `5555555555554444`, CVV `12` (2 dígitos) → esperado: *rechazado por formato CVV*.  
- **Fecha vencida**: PAN válido, fecha anterior al mes/año actual → esperado: *rechazado por expiración*.  
- **Monto inválido (0 o negativo)**: monto `0` o `-10` → esperado: *rechazado por validación de monto*.


---

## 🧑‍💼 Autor

**Nombre:** Edier Güette  
**Rol / Título:** Ing. de Sistemas 
**Correo:** edierjose01@gmail.com  
---


