/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 00:25:59 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 03:43:45 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

/**
 * @file signals.h
 * @brief Signal handling functions prototypes
 *
 */

# include "minishell.h"
# include "structs.h"

void	interactive_sigint_handler(int sig);
void	setup_interactive_signals(void);
void	setup_signals_child(void);

void	heredoc_sigint_handler(int sig);
void	setup_heredoc_signals(void);
void	restore_stdin(void);


/// temporary

void	test_heredoc(t_env *env);

#endif
