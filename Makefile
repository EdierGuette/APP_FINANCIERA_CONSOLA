# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

# Directorios
SRCDIR = src
BUILDDIR = build
TARGET = financiera

# Archivos fuente y objetos
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

# Target por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Regla para compilar cada .c en .o dentro de build/
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crear carpeta build si no existe
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Limpiar binarios y objetos
clean:
	rm -rf $(BUILDDIR) $(TARGET)
	
# Borrar los archivos binario .dat
reset:
	rm -f data/transacciones.dat
	touch data/transacciones.dat

# Crear carpeta data (para configs o BD)
install:
	mkdir -p data

.PHONY: all run clean install
