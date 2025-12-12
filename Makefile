NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -lreadline

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = lib/libft
PRINTF_DIR = lib/ft_printf

# Source files
SRC = $(SRC_DIR)/main_for_tests_2.c \
      $(SRC_DIR)/env/env_init.c \
      $(SRC_DIR)/env/env_operations.c \
      $(SRC_DIR)/env/env_utils.c \
 			$(SRC_DIR)/lexer/token.c \
			$(SRC_DIR)/lexer/quote_handler.c \
			$(SRC_DIR)/lexer/tokenizer_utils.c \
			$(SRC_DIR)/lexer/tokenizer.c \
			$(SRC_DIR)/lexer/expander_utils.c \
			$(SRC_DIR)/lexer/expander.c \
			$(SRC_DIR)/parser/ast_utils.c \
			$(SRC_DIR)/parser/ast.c \
			$(SRC_DIR)/parser/parser_cmd.c \
			$(SRC_DIR)/parser/parser_pipeline.c \
			$(SRC_DIR)/parser/parser.c \
			$(SRC_DIR)/parser/parser_validation.c \
			$(SRC_DIR)/parser/parser_redir.c \
	$(SRC_DIR)/builtins/builtin_echo.c \
	$(SRC_DIR)/builtins/builtin_pwd.c \
	$(SRC_DIR)/builtins/builtin_cd.c \
	$(SRC_DIR)/builtins/builtin_exit.c \
	$(SRC_DIR)/builtins/builtin_env.c \
	$(SRC_DIR)/builtins/builtin_export.c \
	$(SRC_DIR)/builtins/export_utils.c \
	$(SRC_DIR)/builtins/export_ops.c \
	$(SRC_DIR)/builtins/builtin_unset.c

# Object files (with subdirectory structure)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
PRINTF = $(PRINTF_DIR)/libftprintf.a

# Includes and library flags
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR) -I$(PRINTF_DIR)
LIBS = -L$(LIBFT_DIR) -lft -L$(PRINTF_DIR) -lftprintf

all: $(NAME)

$(NAME): $(LIBFT) $(PRINTF) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(LDFLAGS) -o $(NAME)

# Pattern rule that creates subdirectories in obj/ as needed
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(PRINTF):
	@make -C $(PRINTF_DIR)

clean:
	rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@make -C $(PRINTF_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@make -C $(PRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
