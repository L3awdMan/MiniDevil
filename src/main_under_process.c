/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_under_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:01:39 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/11 00:27:10 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"
#include "structs.h"
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
// void	handle_sigint(int sig)
// {
// 	(void)sig;
// 	write(1, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

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
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		setup_signals();
		input = readline("MiniDevil $> ");
		if (input == NULL)
		{
			ft_printf("exit\n"); // could just use ft_putstr_fd() and no need to include ft_printf to the project then (regular printf wouldn't work but our libft version just works with write() so its the same)
			break ;
		}
		if (input[0] != '\0') // could write it as ft_strlen(input) > 0 for better clarity
		{
			add_history(input);
			// HACK : we will parse and excute here
			// ft_printf("I have typed : %s\n", input);
		}
		free(input);
	}
	return (shell.exit_status);
}
