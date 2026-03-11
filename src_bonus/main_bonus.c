/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:01:39 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/09 06:30:41 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include "get_next_line.h"
#include "signals.h"
#include "token.h"
#include "parser.h"
#include "ast.h"
#include "executor.h"
#include "env.h"
#include "minishell_ui.h"

/**
 * @brief tokenize -> defer -> expand -> parse -> collect heredocs -> execute
 * 
 * Key differences with madnatory:
 * - expand_all_tokens() is replaced by defer_expand_tokens()
 * - expand_wildcards() is added before parsing
 * 
 * @param input Raw input string
 * @param shell Shell context
 * @return Exit status of the executed command or 2 on parse error
 * @note Heredocs are collected before the execution begins
 */
static int	process_input(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_ast	*ast;
	int		status;

	tokens = tokenize(input);
	if (!tokens)
		return (2);
	if (defer_expand_tokens(tokens, shell) < 0)
	{
		free_token_list(tokens);
		return (1);
	}
	expand_wildcards(&tokens);
	ast = parse(tokens);
	free_token_list(tokens);
	if (!ast)
		return (2);
	shell->current_ast = ast;
	if (collect_heredocs(ast, shell) == -1)
		status = 128 + SIGINT;
	else
		status = executor(ast, shell);
	free_ast(ast);
	shell->current_ast = NULL;
	return (status);
}

/**
 * @brief Read 1 line of input from the user
 * 
 * Uses readline with a prompt in interactive mode and get_next_line() otherwise
 * (strips the trailing `\n`)
 * 
 * @param shell Shell context
 * @return Newly allocated input line or NULL on EOF
 */
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

/**
 * @brief Filter and dispatch input to processing
 * 
 * - Skips empty and whitespace only inputs
 * - Adds non empty input to readline history in interactive mode
 * 
 * @param input Raw input string
 * @param shell Shell context
 */
static void	handle_input(char *input, t_shell *shell)
{
	int	i;

	if (!input || input[0] == '\0')
		return ;
	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (input[i] == '\0')
		return ;
	if (shell->interactive)
		add_history(input);
	shell->exit_status = process_input(input, shell);
}

/**
 * @brief Main REPL (standard mode)
 * 
 * setup signals -> read input -> handle SIGINT -> process
 * - On EOF (NULL input) it prints "exit" in interactive mode & breaks
 * - It frees input after each iteration
 * 
 * @param shell Shell context (the running flag controls the loop)
 */
static void	main_loop(t_shell *shell)
{
	char	*input;

	shell->running = 1;
	while (shell->running)
	{
		if (shell->interactive)
			setup_interactive_signals();
		g_signal = 0;
		input = read_input(shell);
		if (!input && shell->interactive)
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		if (!input)
			break ;
		if (g_signal == SIGINT)
		{
			shell->exit_status = 130;
			g_signal = 0;
		}
		setup_execution_signals();
		shell->current_input = input;
		handle_input(input, shell);
		free(input);
		shell->current_input = NULL;
	}
}

/**
 * @brief Entry point
 * 
 * Initializes the shell state and eithers enters UI mode (--ui flag)
 * or the standard REPL.
 * - Cleans up env and GNL on exit.
 * 
 * @param ac Argument count
 * @param av Argument vector (--ui triggers UI mode)
 * @param envp Environment passed by the OS
 * @return The shell's final exit status
 */
int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	ft_memset(&shell, 0, sizeof(t_shell));
	shell.env = init_env(envp);
	shell.exit_status = 0;
	shell.interactive = (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO));
	shell.ui_mode = 0;
	shell.is_child = 0;
	shell.ui = NULL;
	if (ac > 1 && ft_strncasecmp(av[1], "--ui", 5) == 0)
		run_ui_mode(&shell);
	else
	{
		(void)ac;
		(void)av;
		main_loop(&shell);
	}
	free_env_list(&shell.env);
	get_next_line(-42);
	return (shell.exit_status);
}
