/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 03:16:21 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 04:43:41 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

/**
 * @file executor.h
 * @brief Executor functions prototypes
 *
 */

# include "minishell.h"
# include "structs.h"

int		handle_heredoc(char *received_delimiter, t_env *env);

char	*find_cmd_path(char *cmd, t_env *env);

void	test_path_finder(t_env *env);

#endif