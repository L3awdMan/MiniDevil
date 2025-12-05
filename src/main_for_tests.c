/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:01:39 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/04 00:12:27 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "token.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>

//--------- Implementation Steps: ---------------------------------
//
// STEP 1: Print newline
//    write(1, "\n", 1);
//    → Moves cursor to next line (like pressing Enter)
//
// STEP 2: Tell readline we're on a new line
//    rl_on_new_line();
//    → Readline function: "hey, cursor moved to new line"
//
// STEP 3: Clear the current input buffer
//    rl_replace_line("", 0);
//    → Replace whatever user typed with empty string
//
// STEP 4: Redraw the prompt
//    rl_redisplay();
//    → Shows "minishell$ " again, ready for new input
//
// Visual example:
//    minishell$ hello worl^C    <- User presses Ctrl+C here
//
//    minishell$                 <- Fresh prompt appears

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

//--------- Setup to follow for implementation : --------------------------

// 1. INFINITE LOOP START
//    Initiate a `while` loop that runs as long as the shell state is running.
//
// 2. CAPTURE INPUT
//    call `input = readline("minishell$ ");`
//
// 3. CHECK FOR EOF (Ctrl+D)
//    IF `input` is NULL:
//       - This means Ctrl+D was pressed.
//       - Print "exit\n" to mimic Bash.
//       - Break the loop (or clean_exit).
//
// 4. CHECK FOR EMPTY INPUT
//    IF `input` is not NULL but points to an empty string (""):
//       - Free `input`.
//    - Continue to the next iteration (don't add to history, don't execute).
//
// 5. HISTORY MANAGEMENT
//    IF `input` contains text:
//       - Call `add_history(input)`.
//       - This allows the Up/Down arrow keys to work immediately.
//
// 6. EXECUTION (Future Step)
//    - This is where you'd send `input` to your parser/executor.
//
// 7. CLEANUP
//    - `free(input)` at the end of every loop iteration.
//
int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)ac;
	(void)av;
	// Initialize environment
	shell.env = init_env(envp);
	shell.exit_status = 0;
	shell.running = 1;
	// Test environment variables
	ft_printf("\n=== ENVIRONMENT TEST ===\n");
	ft_printf("HOME  = %s\n", get_env_value(shell.env, "HOME"));
	ft_printf("USER  = %s\n", get_env_value(shell.env, "USER"));
	ft_printf("PWD   = %s\n", get_env_value(shell.env, "PWD"));
	ft_printf("SHLVL = %s\n", get_env_value(shell.env, "SHLVL"));
	ft_printf("========================\n\n");
    // ========== ADD THIS BLOCK HERE ==========
    ft_printf("\n=== QUOTE HANDLER TEST ===\n");
    
    // Test 1: Single quotes with space
    int is_quoted1 = 0;
    char *result1 = handle_quotes("'hello world'", &is_quoted1);
    ft_printf("Test 1: '%s'\n", "'hello world'");
    ft_printf("Result: '%s', quoted=%d\n\n", result1, is_quoted1);
    free(result1);

    // Test 2: Double quotes
    int is_quoted2 = 0;
    char *result2 = handle_quotes("\"test me\"", &is_quoted2);
    ft_printf("Test 2: \"%s\"\n", "\"test me\"");
    ft_printf("Result: '%s', quoted=%d\n\n", result2, is_quoted2);
    free(result2);

    // Test 3: No quotes
    int is_quoted3 = 0;
    char *result3 = handle_quotes("hello", &is_quoted3);
    ft_printf("Test 3: '%s'\n", "hello");
    ft_printf("Result: '%s', quoted=%d\n\n", result3, is_quoted3);
    free(result3);

    // Test 4: Empty quotes
    int is_quoted4 = 0;
    char *result4 = handle_quotes("''", &is_quoted4);
    ft_printf("Test 4: '%s'\n", "''");
    ft_printf("Result: '%s', quoted=%d\n\n", result4, is_quoted4);
    free(result4);

    // Test 5: Unclosed quote (should print error)
    int is_quoted5 = 0;
    char *result5 = handle_quotes("'unclosed", &is_quoted5);
    ft_printf("Test 5: '%s' (should error above)\n", "'unclosed");
    if (result5)
        free(result5);

    ft_printf("=========================\n\n");
    // ========== END OF TEST BLOCK ==========
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("MiniDevil $> ");
		if (input == NULL)
		{
			ft_printf("exit\n");
			break ;
		}
		if (input[0] != '\0')
		{
			add_history(input);
			// HACK : we will parse and excute here
			// ft_printf("I have typed : %s\n", input);
		}
		free(input);
	}
	return (shell.exit_status);
}
