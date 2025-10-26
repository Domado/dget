CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99 
LDFLAGS = 

LIB_SRCS = daiso_http.c daiso_net.c daiso_request.c daiso_response.c
LIB_OBJS = $(LIB_SRCS:.c=.o)

MAIN_SRC = main.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)

TARGET_LIB = libdaisohttp.a
TARGET_EXE = http_client_test

.PHONY: all clean

all: $(TARGET_EXE)

$(TARGET_EXE): $(MAIN_OBJ) $(TARGET_LIB)
	$(CC) $(CFLAGS) $(MAIN_OBJ) -o $(TARGET_EXE) $(TARGET_LIB) $(LDFLAGS)

$(TARGET_LIB): $(LIB_OBJS)
	ar rcs $(TARGET_LIB) $(LIB_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_OBJS) $(MAIN_OBJ) $(TARGET_LIB) $(TARGET_EXE)
