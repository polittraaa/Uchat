NAME = db
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./inc -I/home/poli3/UCODE_UCHAT/Code/client/libs/libmx/inc

# Paths
LIBMX_DIR = /home/poli3/UCODE_UCHAT/Code/client/libs/libmx
LIBS = -L$(LIBMX_DIR) -lmx -lsqlite3

# Source files
SRC = main.c your_library.c
OBJ = $(SRC:.c=.o)

# Build target
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(NAME)


