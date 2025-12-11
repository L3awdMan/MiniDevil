/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 00:25:59 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/11 01:04:01 by baelgadi         ###   ########.fr       */
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

void	handle_sigint(int sig);
void	setup_signals(void);
void	setup_signals_child(void);
void	setup_signals_heredoc(void);

#endif
