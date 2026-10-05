# Compiladores
CC=gcc
CXX=g++

# Flags
CFLAGS=-c
CXXFLAGS=-c -std=c++11
LDFLAGS=-lws2_32

# Carpeta para objetos
OBJDIR=build

# Archivos fuente
C_SOURCES=$(wildcard *.c)
CPP_SOURCES=$(wildcard *.cpp)

# Archivos objeto en carpeta build/
C_OBJECTS=$(addprefix $(OBJDIR)/, $(notdir $(C_SOURCES:.c=.o)))
CPP_OBJECTS=$(addprefix $(OBJDIR)/, $(notdir $(CPP_SOURCES:.cpp=.o)))

# Ejecutable
TARGET=Server.exe

# Regla principal
all: $(OBJDIR) $(TARGET)

# Crear carpeta build si no existe
$(OBJDIR):
	mkdir $(OBJDIR)

# Enlazado
$(TARGET): $(C_OBJECTS) $(CPP_OBJECTS)
	$(CXX) $(C_OBJECTS) $(CPP_OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compilar .c → build/*.o
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Compilar .cpp → build/*.o
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Limpiar objetos y binario
clean:
	del /Q $(OBJDIR)\*.o $(TARGET) 2>nul || true
