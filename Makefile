# ─────────────────────────────────
#             🎨 COLORS
# ─────────────────────────────────

RESET = \033[0m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
GRAY = \033[90m
ORANGE = \033[38;5;208m
BRIGHT_RED = \033[38;5;196m
GOLD = \033[38;5;220m
UNDERLINE = \033[4m

# ─────────────────────────────────
#         🛠️ CONFIGURATION
# ─────────────────────────────────

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -lreadline

SRC_DIR = src
SRC_BONUS_DIR = src_bonus
OBJ_DIR = obj
OBJ_BONUS_DIR = obj_bonus
INC_DIR = includes
LIBFT_DIR = libft

INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR)/include
LIBS = -L$(LIBFT_DIR) -lft
LIBFT = $(LIBFT_DIR)/libft.a

# ─────────────────────────────────
#               📂 FILES
# ─────────────────────────────────

# ─── MANDATORY ───────────────────

SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/lexer/tokenizer.c \
      $(SRC_DIR)/lexer/tokenizer_utils.c \
      $(SRC_DIR)/lexer/token.c \
      $(SRC_DIR)/lexer/expander.c \
      $(SRC_DIR)/lexer/expander_utils.c \
      $(SRC_DIR)/lexer/tilde_expand.c \
      $(SRC_DIR)/parser/parser.c \
      $(SRC_DIR)/parser/parser_cmd.c \
      $(SRC_DIR)/parser/parser_pipeline.c \
      $(SRC_DIR)/parser/parser_redir.c \
      $(SRC_DIR)/parser/parser_redir_utils.c \
      $(SRC_DIR)/parser/parser_grammar.c \
      $(SRC_DIR)/parser/parser_grammar_error.c \
      $(SRC_DIR)/parser/ast.c \
      $(SRC_DIR)/parser/ast_utils.c \
      $(SRC_DIR)/executor/executor.c \
      $(SRC_DIR)/executor/executor_pipe.c \
      $(SRC_DIR)/executor/executor_redir.c \
      $(SRC_DIR)/executor/exec_cmd.c \
      $(SRC_DIR)/executor/exec_utils.c \
      $(SRC_DIR)/executor/path.c \
      $(SRC_DIR)/executor/heredoc.c \
      $(SRC_DIR)/executor/heredoc_collect.c \
      $(SRC_DIR)/builtins/builtin_echo.c \
      $(SRC_DIR)/builtins/builtin_pwd.c \
      $(SRC_DIR)/builtins/builtin_cd.c \
      $(SRC_DIR)/builtins/builtin_exit.c \
      $(SRC_DIR)/builtins/builtin_env.c \
      $(SRC_DIR)/builtins/builtin_export.c \
      $(SRC_DIR)/builtins/builtin_unset.c \
      $(SRC_DIR)/builtins/export_utils.c \
      $(SRC_DIR)/builtins/export_ops.c \
      $(SRC_DIR)/env/env_init.c \
      $(SRC_DIR)/env/env_operations.c \
      $(SRC_DIR)/env/env_utils.c \
      $(SRC_DIR)/env/env_conversion.c \
      $(SRC_DIR)/signals/signals.c \
      $(SRC_DIR)/signals/heredoc_signals.c

# ─── UI ──────────────────────────

UI_SRC = $(SRC_DIR)/ui/core/ui_main.c \
         $(SRC_DIR)/ui/core/terminal.c \
         $(SRC_DIR)/ui/core/terminal_init.c \
         $(SRC_DIR)/ui/core/ui_signals.c \
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

SRC += $(UI_SRC)

# ─── BONUS SHARED (from src) ─────

BONUS_SHARED = $(SRC_DIR)/lexer/token.c \
               $(SRC_DIR)/lexer/expander.c \
               $(SRC_DIR)/lexer/expander_utils.c \
               $(SRC_DIR)/lexer/tilde_expand.c \
               $(SRC_DIR)/parser/parser_cmd.c \
               $(SRC_DIR)/parser/parser_redir_utils.c \
               $(SRC_DIR)/executor/executor_pipe.c \
               $(SRC_DIR)/executor/executor_redir.c \
               $(SRC_DIR)/executor/exec_cmd.c \
               $(SRC_DIR)/executor/exec_utils.c \
               $(SRC_DIR)/executor/path.c \
               $(SRC_DIR)/executor/heredoc.c \
               $(SRC_DIR)/builtins/builtin_echo.c \
               $(SRC_DIR)/builtins/builtin_pwd.c \
               $(SRC_DIR)/builtins/builtin_cd.c \
               $(SRC_DIR)/builtins/builtin_exit.c \
               $(SRC_DIR)/builtins/builtin_env.c \
               $(SRC_DIR)/builtins/builtin_export.c \
               $(SRC_DIR)/builtins/builtin_unset.c \
               $(SRC_DIR)/builtins/export_utils.c \
               $(SRC_DIR)/builtins/export_ops.c \
               $(SRC_DIR)/env/env_init.c \
               $(SRC_DIR)/env/env_operations.c \
               $(SRC_DIR)/env/env_utils.c \
               $(SRC_DIR)/env/env_conversion.c \
               $(SRC_DIR)/signals/signals.c \
               $(SRC_DIR)/signals/heredoc_signals.c \
               $(SRC_DIR)/lexer/tokenizer.c \
               $(UI_SRC)

# ─── BONUS NEW ───────────────────

BONUS_NEW = $(SRC_BONUS_DIR)/main_bonus.c \
            $(SRC_BONUS_DIR)/lexer/tokenizer_utils_bonus.c \
            $(SRC_BONUS_DIR)/lexer/expander_bonus.c \
            $(SRC_BONUS_DIR)/lexer/wildcard_bonus.c \
            $(SRC_BONUS_DIR)/lexer/wildcard_match_bonus.c \
            $(SRC_BONUS_DIR)/lexer/wildcard_replace_bonus.c \
            $(SRC_BONUS_DIR)/parser/parser_bonus.c \
            $(SRC_BONUS_DIR)/parser/parser_grammar_bonus.c \
            $(SRC_BONUS_DIR)/parser/parser_grammar_error_bonus.c \
            $(SRC_BONUS_DIR)/parser/parser_pipeline_bonus.c \
            $(SRC_BONUS_DIR)/parser/parser_redir_bonus.c \
            $(SRC_BONUS_DIR)/parser/parser_subshell_bonus.c \
            $(SRC_BONUS_DIR)/parser/ast_bonus.c \
            $(SRC_BONUS_DIR)/parser/ast_utils_bonus.c \
            $(SRC_BONUS_DIR)/executor/executor_bonus.c \
            $(SRC_BONUS_DIR)/executor/executor_logic_bonus.c \
            $(SRC_BONUS_DIR)/executor/heredoc_collect_bonus.c

# ─────────────────────────────────
#      🚧 SETUP & DEPENDENCIES
# ─────────────────────────────────

OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

BONUS_OBJ_SHARED = $(BONUS_SHARED:$(SRC_DIR)/%.c=$(OBJ_BONUS_DIR)/%.o)
BONUS_OBJ_NEW = $(BONUS_NEW:$(SRC_BONUS_DIR)/%.c=$(OBJ_BONUS_DIR)/bonus/%.o)
BONUS_OBJ = $(BONUS_OBJ_SHARED) $(BONUS_OBJ_NEW)

DEP = $(OBJ:.o=.d)
BONUS_DEP = $(BONUS_OBJ:.o=.d)

# ─────────────────────────────────
#            🎯 TARGETS
# ─────────────────────────────────

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@rm -f .bonus_stamp
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(LDFLAGS) -o $(NAME) \
		&& printf "$(GREEN)✅ $(NAME) created$(RESET)\n" \
		|| printf "$(RED)❌ $(NAME) link failed$(RESET)\n"
	@printf "\n$(BRIGHT_RED)"
	@printf "  ╔═════════════════════════════════════════════════════╗\n"
	@printf "  ║$(GOLD)       M I N I D E V I L    I S    R E A D Y         $(BRIGHT_RED)║\n"
	@printf "  ╠═════════════════════════════════════════════════════╣\n"
	@printf "  ║$(ORANGE)   run ./minishell --ui  to enable the DEVIL MODE    $(BRIGHT_RED)║\n"
	@printf "  ║$(RESET)   run ./minishell for the standard mode             $(BRIGHT_RED)║\n"
	@printf "  ╚═════════════════════════════════════════════════════╝$(RESET)\n\n"

bonus: .bonus_stamp

.bonus_stamp: $(LIBFT) $(BONUS_OBJ)
	@rm -f $(NAME)
	@$(CC) $(CFLAGS) $(BONUS_OBJ) $(LIBS) $(LDFLAGS) -o $(NAME) \
		&& printf "$(GREEN)✅ $(NAME) (bonus) created$(RESET)\n" \
		|| printf "$(RED)❌ $(NAME) (bonus) link failed$(RESET)\n"
	@touch .bonus_stamp
	@printf "\n$(BRIGHT_RED)"
	@printf "  ╔═════════════════════════════════════════════════════╗\n"
	@printf "  ║$(GOLD)    M I N I D E V I L    B O N U S    R E A D Y      $(BRIGHT_RED)║\n"
	@printf "  ╠═════════════════════════════════════════════════════╣\n"
	@printf "  ║$(ORANGE)       Bonus: && || () and * wildcards enabled       $(BRIGHT_RED)║\n"
	@printf "  ╚═════════════════════════════════════════════════════╝$(RESET)\n\n"

# ─────────────────────────────────
#        💽 COMPILATION RULES
# ─────────────────────────────────

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "Compiling $(BLUE)%-45s$(RESET)" $<
	@$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@ \
		&& printf "✅\n" || printf "❌\n"

$(OBJ_BONUS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "Compiling $(CYAN)%-45s$(RESET)" $<
	@$(CC) $(CFLAGS) -DBONUS $(INCLUDES) -MMD -MP -c $< -o $@ \
		&& printf "✅\n" || printf "❌\n"

$(OBJ_BONUS_DIR)/bonus/%.o: $(SRC_BONUS_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "Compiling $(MAGENTA)%-45s$(RESET)" $<
	@$(CC) $(CFLAGS) -DBONUS $(INCLUDES) -MMD -MP -c $< -o $@ \
		&& printf "✅\n" || printf "❌\n"

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

# ─────────────────────────────────
#            🧹 CLEANING
# ─────────────────────────────────

clean:
	@make -s -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR) $(OBJ_BONUS_DIR) .bonus_stamp
	@echo "$(YELLOW)🧹 Objects and dependencies deleted (clean)$(RESET)"

fclean: clean
	@make -s -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@echo "$(RED)❌ $(NAME) removed (fclean)$(RESET)"

re: fclean all

# ─────────────────────────────────
#          📖 DOCUMENTATION
# ─────────────────────────────────

doc:
	@cd doc && doxygen Doxyfile
	@sh doc/theme/theme_graphs.sh doc/html

# ─────────────────────────────────
#         🌐 HELP & FOOTER
# ─────────────────────────────────

help:
	@echo "$(BLUE)Available targets:$(RESET)"
	@echo "$(YELLOW)all      $(RESET)– Build minishell (mandatory)"
	@echo "$(YELLOW)bonus    $(RESET)– Build minishell (bonus: && || () *)"
	@echo "$(YELLOW)clean    $(RESET)– Delete .o and .d files"
	@echo "$(YELLOW)fclean   $(RESET)– Full clean + remove binary"
	@echo "$(YELLOW)re       $(RESET)– fclean + all"
	@echo "$(YELLOW)doc      $(RESET)– Generate Doxygen documentation"

-include $(DEP)
-include $(BONUS_DEP)

.PHONY: all clean fclean re bonus doc help