/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:03:44 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 11:26:33 by zotaj-di         ###   ########.fr       */
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

///// TEMPORARY

void	test_echo(void);

void	test_pwd(void);

void	test_cd(t_env **env);

void	test_exit(void);

void	test_env(t_env *env);
void	test_env_hidden(t_env **env);

void	test_export(t_env **env);

void	test_unset(t_env **env);

void	test_path_finder(t_env *env);

#endif
