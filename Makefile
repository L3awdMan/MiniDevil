NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -lreadline

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = libft

# Source files
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/env/env_init.c \
      $(SRC_DIR)/env/env_operations.c \
      $(SRC_DIR)/env/env_utils.c \
 			$(SRC_DIR)/lexer/token.c \
			$(SRC_DIR)/lexer/quote_handler.c \
			$(SRC_DIR)/lexer/tokenizer_utils.c \
			$(SRC_DIR)/lexer/tokenizer.c \
			$(SRC_DIR)/lexer/expander_utils.c \
			$(SRC_DIR)/lexer/expander.c \
			$(SRC_DIR)/lexer/tilde_expand.c \
			$(SRC_DIR)/parser/ast_utils.c \
			$(SRC_DIR)/parser/ast.c \
			$(SRC_DIR)/parser/parser_cmd.c \
			$(SRC_DIR)/parser/parser_pipeline.c \
			$(SRC_DIR)/parser/parser.c \
			$(SRC_DIR)/parser/parser_grammar.c \
			$(SRC_DIR)/parser/parser_grammar_error.c \
			$(SRC_DIR)/parser/parser_redir.c \
	$(SRC_DIR)/builtins/builtin_echo.c \
	$(SRC_DIR)/builtins/builtin_pwd.c \
	$(SRC_DIR)/builtins/builtin_cd.c \
	$(SRC_DIR)/builtins/builtin_exit.c \
	$(SRC_DIR)/builtins/builtin_env.c \
	$(SRC_DIR)/builtins/builtin_export.c \
	$(SRC_DIR)/builtins/export_utils.c \
	$(SRC_DIR)/builtins/export_ops.c \
	$(SRC_DIR)/builtins/builtin_unset.c \
	$(SRC_DIR)/signals/signals.c \
	$(SRC_DIR)/signals/heredoc_signals.c \
	$(SRC_DIR)/executor/heredoc.c \
	$(SRC_DIR)/executor/path.c \
	$(SRC_DIR)/env/env_conversion.c \
	$(SRC_DIR)/executor/exec_cmd.c \
	$(SRC_DIR)/executor/exec_utils.c \
			$(SRC_DIR)/executor/executor.c \
			$(SRC_DIR)/executor/executor_pipe.c \
			$(SRC_DIR)/executor/executor_redir.c

# UI Source files
UI_SRC = $(SRC_DIR)/ui/core/ui_main.c \
         $(SRC_DIR)/ui/core/terminal.c \
         $(SRC_DIR)/ui/core/terminal_init.c \
         $(SRC_DIR)/ui/core/signals.c \
         $(SRC_DIR)/ui/core/output.c \
         $(SRC_DIR)/ui/core/waifu.c \
         $(SRC_DIR)/ui/core/waifu_art.c \
         $(SRC_DIR)/ui/input/input.c \
         $(SRC_DIR)/ui/input/command.c \
         $(SRC_DIR)/ui/input/execute.c \
         $(SRC_DIR)/ui/input/execute_utils.c \
         $(SRC_DIR)/ui/draw/drawing.c \
         $(SRC_DIR)/ui/draw/drawing_box.c \
         $(SRC_DIR)/ui/draw/drawing_output.c \
         $(SRC_DIR)/ui/draw/drawing_text.c \
         $(SRC_DIR)/ui/draw/drawing_utils.c \
         $(SRC_DIR)/ui/welcome/welcome.c \
         $(SRC_DIR)/ui/welcome/welcome_demon.c \
         $(SRC_DIR)/ui/welcome/welcome_effects.c \
         $(SRC_DIR)/ui/welcome/welcome_loading.c \
         $(SRC_DIR)/ui/welcome/welcome_utils.c

# Add UI sources to main sources
SRC += $(UI_SRC)


# Object files (with subdirectory structure)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a

# Includes and library flags
INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR)/include
LIBS = -L$(LIBFT_DIR) -lft

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(LDFLAGS) -o $(NAME)

# Pattern rule that creates subdirectories in obj/ as needed
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
