/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:43:02 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:40:06 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

# include "ft_printf.h"
# include "libft.h"
# include "get_next_line.h"

# include "ast.h"
# include "env.h"
# include "parser.h"
# include "structs.h"
# include "token.h"
# include "signals.h"
# include "builtins.h"
# include "executor.h"

extern volatile sig_atomic_t	g_signal;

#endif
