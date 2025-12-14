/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 03:16:21 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/14 05:17:11 by baelgadi         ###   ########.fr       */
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

int		exec_external(char **args, t_env *env);
int		exec_cmd_not_found(char *cmd);
int		is_builtin(char *cmd);
int		exec_builtin(char **args, t_env **env, int last_status);
int		exec_simple_command(char **args, t_env **env, int last_status);


/// TEMPORARY
void	test_heredoc(t_env *env);
void	test_path_finder(t_env *env);
void	test_exec_external(t_env **env);

#endif
