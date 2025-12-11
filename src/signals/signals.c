/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 23:44:56 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/11 05:24:05 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t g_signal = 0;

/**
 * @brief Handler for SIGINT in interactive mode (waiting for input)
 * 
 * When the shell is waiting for user input (readline)
 * 1. Update the global signal status
 * 2. Move to a new line
 * 3. Clear current readline buffer
 * 4. Redisplay the prompt
 */
void	handle_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Set up signal handlers for the interactive mode
 * Rules:
 * - SIGQUIT = ignore completely
 * - SIGINT = use sigint_handler() to refresh the prompt
 * 
 * Uses SA_RESTART for SIGINT to ensure sys calls are restarted if interrupted
 * @note We could use only one struct, but this way is cleaner
 */
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * @brief Restore default signal handling (SIG_DFL) for a child process
 * 
 * Called after fork() and before execve() to reset SIGINT and SIGQUIT to
 * SIG_DFL so that external commands receive Ctrl C and Ctrl \ normally
 */
void	reset_child_signals(void) // When 
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_signals_heredoc(void)
{
	// need to handle SIGINT inside heredoc
}
