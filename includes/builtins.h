/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:03:44 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/23 22:40:52 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int		builtin_echo(char **args);

int		builtin_pwd(void);

int		builtin_cd(char **args, t_env **env);

int		builtin_exit(char **args, t_shell *shell);

int		builtin_env(char **args, t_env *env);

int		builtin_export(char **args, t_env **env);

void	print_sorted_export(t_env *env);

void	export_assign(t_env **env, char *arg);

void	export_append(t_env **env, char *arg);

void	export_no_value(t_env **env, char *key);

int		builtin_unset(char **args, t_env **env);

#endif
