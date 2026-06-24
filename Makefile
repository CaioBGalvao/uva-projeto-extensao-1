CC = gcc
CFLAGS = -Wall -Wextra -Isrc -g
LDFLAGS = -lreadline -lhistory

SRCS = src/main.c src/cliente.c src/produto.c src/pedido.c src/devolucao.c src/relatorio.c src/persistencia.c src/input.c src/lookup.c src/logger.c
OBJS = $(SRCS:.c=.o)
TARGET = app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
