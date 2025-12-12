/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 22:35:52 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/12 23:46:30 by baelgadi         ###   ########.fr       */
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
 * - For SIGINT we use custom handler to close SSTDIN (thus breaking readline)
 * - For SIGQUIT we ignore (it should do nothing in heredoc)
 */
void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
