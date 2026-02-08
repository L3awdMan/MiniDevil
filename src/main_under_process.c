/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_under_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:01:39 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/08 20:15:59 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// temporarily code to debug
void	debug_print_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	printf("\n=== TOKENS DEBUG ===\n");
	while (tokens)
	{
		printf("[%d] value=\"%s\" quote=%d connected=%d\n", i, tokens->value,
			tokens->quote_type, tokens->connected);
		tokens = tokens->next;
		i++;
	}
	printf("====================\n\n");
}

//==================== FUNCTION: process_input ===========================
//
// PURPOSE:
//    Process user input through the full pipeline:
//    tokenize → parse → execute
//
// RETURN:
//    int - Exit status from execution
//
// PARAMETERS:
//    char *input     - User input string
//    t_shell *shell  - Shell state
//
// VARIABLES:
//    t_token *tokens - Token list from lexer
//    t_ast *ast      - Abstract syntax tree from parser
//    int status      - Exit status from execution
//
// ALGORITHM:
//    1. Tokenize input string
//    2. If tokenization fails: return error
//    3. Parse tokens into AST
//    4. Free tokens (no longer needed)
//    5. If parsing fails: return error
//    6. Execute AST
//    7. Free AST
//    8. Return exit status

static int	process_input(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_ast	*ast;
	int		status;

	tokens = tokenize(input);
	// debug_print_tokens(tokens);
	if (!tokens)
		return (2);
	if (expand_all_tokens(tokens, shell) < 0) // <-- ADDED THIS
		return (free_token_list(tokens), 1);  // <-- ADDED THIS
	ast = parse(tokens);
	free_token_list(tokens);
	if (!ast)
		return (2);
	status = executor(ast, shell);
	free_ast(ast);
	return (status);
}

//==================== FUNCTION: read_input ==============================
//
// PURPOSE:
//    Read input based on whether shell is in interactive mode
//
// RETURN:
//    char* - Input line or NULL on EOF
//
// PARAMETERS:
//    t_shell *shell - Shell state
//
// ALGORITHM:
//    1. If interactive: use readline with prompt
//    2. If non-interactive: use get_next_line and strip newline
//    3. Return line or NULL on EOF

static char	*read_input(t_shell *shell)
{
	char	*line;
	int		len;

	if (shell->interactive)
		return (readline("MiniDevil $> "));
	line = get_next_line(STDIN_FILENO);
	if (line)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
	}
	return (line);
}

//==================== FUNCTION: handle_input ============================
//
// PURPOSE:
//    Handle user input: add to history and execute
//
// RETURN:
//    void
//
// PARAMETERS:
//    char *input    - User input string
//    t_shell *shell - Shell state
//
// ALGORITHM:
//    1. Check if input is empty
//    2. If not empty: add to history
//    3. Process through pipeline
//    4. Update shell exit status

static void	handle_input(char *input, t_shell *shell)
{
	if (!input || input[0] == '\0')
		return ;
	if (shell->interactive)
		add_history(input);
	shell->exit_status = process_input(input, shell);
}

//==================== FUNCTION: main_loop ===============================
//
// PURPOSE:
//    Main REPL loop: Read-Eval-Print Loop
//
// RETURN:
//    void
//
// PARAMETERS:
//    t_shell *shell - Shell state
//
// VARIABLES:
//    char *input - User input from readline
//
// ALGORITHM:
//    1. Setup signal handlers
//    2. Read line from user
//    3. If NULL (Ctrl+D): print exit and break
//    4. Handle the input
//    5. Free input
//    6. Repeat

static void	main_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		if (shell->interactive)
			setup_interactive_signals();
		g_signal = 0;
		input = read_input(shell);
		if (!input)
		{
			if (shell->interactive)
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		handle_input(input, shell);
		free(input);
	}
}

//==================== FUNCTION: main ====================================
//
// PURPOSE:
//    Entry point - initialize shell and start main loop
//
// RETURN:
//    int - Final exit status
//
// PARAMETERS:
//    int ac          - Argument count (unused)
//    char **av       - Argument vector (unused)
//    char **envp     - Environment variables
//
// VARIABLES:
//    t_shell shell - Shell state structure
//
// ALGORITHM:
//    1. Initialize environment from envp
//    2. Initialize exit status to 0
//    3. Start main loop
//    4. Cleanup: free environment
//    5. Return final exit status

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	shell.env = init_env(envp);
	shell.exit_status = 0;
	shell.interactive = (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO));
	main_loop(&shell);
	free_env_list(&shell.env);
	return (shell.exit_status);
}
