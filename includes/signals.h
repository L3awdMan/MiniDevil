/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 00:25:59 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/04 04:32:23 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>

/**
 * @brief The single global variable
 * 
 * Set by signal handlers and checked/cleared in the main loop.
 * Only stores signal numbers.
 */
extern volatile sig_atomic_t	g_signal;

void	interactive_sigint_handler(int sig);

void	setup_interactive_signals(void);

void	reset_child_signals(void);

void	setup_execution_signals(void);

void	heredoc_sigint_handler(int sig);

void	setup_heredoc_signals(void);

void	restore_stdin(void);

#endif
