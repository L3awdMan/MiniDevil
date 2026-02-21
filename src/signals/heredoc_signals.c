/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 22:35:52 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/21 08:07:43 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Signal handler for heredoc mode
 * 
 * When Ctrl C is pressed during heredoc:
 * - Set g_signal to SIGINT
 * - Write newline to STDOUT
 * - Close STDIN forcing readline to return NULL
 */
void	heredoc_sigint_handler(int sig)
{
	g_signal = sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	close(STDIN_FILENO);
}

/**
 * @brief Configure signals for heredoc execution
 * 
 * No SA_RESTART because it would make readline silently retry after the signal,
 * preventing heredoc from stopping on CTRL C
 */
void	setup_heredoc_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * @brief Restore STDIN after heredoc interruption
 * 
 * When SIGINT interrupts the heredoc, our handler closes STDIN to force
 * readline() to stop waiting. The problem is that this leaves file descriptor 0
 * closed and could cause the shell loop to exit directly
 * This function repairs STDIN easily:
 * 1. Open /dev/tty
 * @note /dev/tty is a special file that always points to the terminal that
 * started the current process
 * 2. Use dup2() to clone the new tty fd onto STDIN
 * 3. Close the temporary file descriptor
 */
void	restore_stdin(void)
{
	int	tty_fd;

	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd != -1)
	{
		dup2(tty_fd, STDIN_FILENO);
		close(tty_fd);
	}
}
