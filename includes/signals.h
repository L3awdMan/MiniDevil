/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 00:25:59 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/25 05:10:27 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"
# include "structs.h"

void	interactive_sigint_handler(int sig);

void	setup_interactive_signals(void);

void	reset_child_signals(void);

void	setup_execution_signals(void);

void	heredoc_sigint_handler(int sig);

void	setup_heredoc_signals(void);

void	restore_stdin(void);

#endif
