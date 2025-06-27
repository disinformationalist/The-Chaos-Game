# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jhotchki <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/23 12:09:08 by jhotchki          #+#    #+#              #
#    Updated: 2024/02/23 12:09:11 by jhotchki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := chaos

SRC_DIR = ./src
OBJ_DIR = ./obj
INC_DIR = ./inc

SRCS := ./src/main.c \
./src/chaos.c \
./src/colors.c \
./src/events.c \
./src/events_rules.c \
./src/events_mouse_super.c \
./src/events_other.c \
./src/init.c \
./src/rules.c \
./src/set_polygon.c \
./src/intermed.c \
./src/print_utils.c \
./src/print_utils_2.c \
./src/games.c \
./src/set_png_board.c \
./src/set_godmode_data.c \
./src/get_god_data.c \
./src/resize_window.c \
./src/time_utils.c

  
CFLAGS := -Wall -Wextra -Werror -I$(INC_DIR) -Ofast -march=native -Wno-unused-result

CC := cc

OBS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


#OBS := $(SRCS:.c=.o)

COLOR_RESET = \033[0m
COLOR_GREEN = \033[1;92m
COLOR_BLUE = \033[1;96m

MAKEFLAGS += --no-print-directory

define print_colored
	@echo "$(COLOR_GREEN)$(1)$(COLOR_BLUE)$(2)$(COLOR_RESET)$(3)"
endef

.SILENT:

all: $(NAME)

MLX_PATH := ./minilibx-linux

$(NAME): build_mlx $(OBS)
	$(CC) $(CFLAGS) $(OBS) -lm -Limage_processing -limage_processing -Lminilibx-linux -lmlx_Linux -lX11 -lXext -o $(NAME) -lpng
	cp $(NAME) ../$(NAME)
	$(call print_colored, "[SUCCESS]", "./$(NAME)", "Ready")

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

build_mlx:
	$(MAKE) -C $(MLX_PATH)

clean:
	rm -rf $(OBS)

fclean: clean
	rm -rf	$(NAME)
	
re: fclean all

.PHONY: all clean fclean re
