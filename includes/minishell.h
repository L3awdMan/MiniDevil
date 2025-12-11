/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:43:02 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/11 00:25:44 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/**
 * @file minishell.h
 * @brief Main header file, includes all system libraries and project headers
 *
 * The central hub of the project
 */

//==================================================
//================ SYSTEM INCLUDES =================
//==================================================

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

//==================================================
//================ LIBFT INCLUDES ==================
//==================================================

# include "ft_printf.h"
# include "libft.h"

//==================================================
//=============== GLOBAL VARIABLES =================
//==================================================

/**
 * @brief Global signal indicator
 *
 * Used to communicate between signal handlers and main program
 * Only modified in signal handlers and read in main loop
 * sig_atomic_t ensures atomic read/write operations (@see WIP)
 */
extern volatile sig_atomic_t	g_signal;

//==================================================
//=============== PROJECT INCLUDES =================
//==================================================

# include "ast.h"
# include "env.h"
# include "parser.h"
# include "structs.h"
# include "token.h"
# include "signals.h"

#endif
