CC_LINUX = gcc
CC_WIN = x86_64-w64-mingw32-gcc

CFLAGS_LINUX = -Wall -Wextra -Isrc -Ivendor/isocline -O3
CFLAGS_WIN = -Wall -Wextra -Isrc -Ivendor/isocline -O3

LDFLAGS_LINUX = 
LDFLAGS_WIN = 

SRCS = src/main.c src/cliente.c src/produto.c src/pedido.c src/devolucao.c src/relatorio.c src/persistencia.c src/input.c src/lookup.c src/logger.c vendor/isocline/isocline.c

OBJS_LINUX = $(SRCS:.c=.o)
OBJS_WIN = $(SRCS:.c=.win.o)

TARGET_LINUX = app
TARGET_WIN = app.exe

all: linux windows

linux: $(TARGET_LINUX)

windows: $(TARGET_WIN)

$(TARGET_LINUX): $(OBJS_LINUX)
	$(CC_LINUX) $(CFLAGS_LINUX) -o $@ $^ $(LDFLAGS_LINUX)

$(TARGET_WIN): $(OBJS_WIN)
	$(CC_WIN) $(CFLAGS_WIN) -o $@ $^ $(LDFLAGS_WIN)

%.o: %.c
	$(CC_LINUX) $(CFLAGS_LINUX) -c -o $@ $<

%.win.o: %.c
	$(CC_WIN) $(CFLAGS_WIN) -c -o $@ $<

clean:
	rm -f $(OBJS_LINUX) $(OBJS_WIN) $(TARGET_LINUX) $(TARGET_WIN)
