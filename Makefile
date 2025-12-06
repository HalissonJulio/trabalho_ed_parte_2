# --- Compilador e Flags ---
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic

# --- Arquivos do Projeto ---
SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)
DEPS = $(shell find . -type f -name '*.h')
OBJS = $(SRCS:.c=.o)
TARGET = campeonato

# --- Regras de Compilação ---

# Regra principal (padrão): compila o executável final
all: $(TARGET)

# Regra para "linkar" o executável final
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# Regra genérica para compilar .c em .o
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilação de depuração (otimização desligada)
debug: CFLAGS += -O0
debug: $(OBJS)
	$(CC) $(OBJS) -o campeonato-debug

# Regra para limpar os arquivos compilados
clean:
	rm -f $(OBJS) $(TARGET) campeonato-debug