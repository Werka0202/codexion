NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -pthread

SRC_DIR = src
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/init.c \
      $(SRC_DIR)/monitor.c \
      $(SRC_DIR)/routine.c \
      $(SRC_DIR)/time.c \
      $(SRC_DIR)/utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
