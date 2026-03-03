/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 22:35:52 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/03 08:07:32 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief SIGINT handler for heredoc
 * 
 * Closes STDIN to force readline/gnl to return NULL, thus breaking the heredoc
 * read loop
 * 
 * @param sig Signal number (SIGINT)
 * @note STDIN is later restored via restore_stdin()
 */
void	heredoc_sigint_handler(int sig)
{
	g_signal = sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	close(STDIN_FILENO);
}

/**
 * @brief Configure signals for heredoc
 * 
 * - SIGINT calls heredoc_sigint_handler() without SA_RESTART so readline
 * returns immediately
 * - SIGQUIT ignored
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
 * @brief Restore STDIN after heredoc SIGINT closed it
 * 
 * Reopens `/dev/tty` and uses dup2 to reassign it to fd 0 (STDIN)
 */
void	restore_stdin(void)
{
	int	tty_fd;

	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd != -1 && tty_fd != STDIN_FILENO)
	{
		dup2(tty_fd, STDIN_FILENO);
		close(tty_fd);
	}
}
