/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_under_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:01:39 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/19 00:03:32 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	t_token *tmp; // for debug
	tokens = tokenize(input);
	if (!tokens)
		return (1);
	// for debug
	printf("\n===================\n");
	tmp = tokens;
	while (tmp)
	{
		printf("type : %d -- value : %s\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}
	printf("\n===================\n");
	ast = parse(tokens);
	free_token_list(tokens);
	if (!ast)
		return (1);
	status = executor(ast, shell);
	free_ast(ast);
	return (status);
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
		setup_interactive_signals();
		input = readline("MiniDevil $> ");
		if (!input)
		{
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
	main_loop(&shell);
	free_env_list(&shell.env);
	return (shell.exit_status);
}
