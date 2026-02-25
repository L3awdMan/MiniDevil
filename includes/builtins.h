/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:03:44 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/25 05:10:09 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

//────────────── builtin_echo.c ────────────────

int		builtin_echo(char **args);

//────────────── builtin_pwd.c ────────────────

int		builtin_pwd(void);

//────────────── builtin_cd.c ────────────────

int		builtin_cd(char **args, t_env **env);

//────────────── builtin_exit.c ────────────────

int		builtin_exit(char **args, t_shell *shell);

//────────────── builtin_env.c ────────────────

int		builtin_env(char **args, t_env *env);

//────────────── builtin_export.c ────────────────

int		builtin_export(char **args, t_env **env);

//────────────── export_utils.c ────────────────

void	print_sorted_export(t_env *env);

//────────────── export_ops.c ────────────────

void	export_assign(t_env **env, char *arg);

void	export_append(t_env **env, char *arg);

void	export_no_value(t_env **env, char *key);

//────────────── builtin_unset.c ────────────────

int		builtin_unset(char **args, t_env **env);

#endif
