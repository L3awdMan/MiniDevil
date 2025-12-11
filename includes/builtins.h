/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:03:44 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/11 23:21:10 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int builtin_echo(char **args);

int	builtin_pwd(void);

int	builtin_cd(char **args, t_env **env);

int	builtin_exit(char **args, int last_status);

int	builtin_env(t_env *env);


///// TEMPORARY

void	test_echo(void);

void	test_pwd(void);

void	test_cd(t_env **env);

void	test_exit(void);

void	test_env(t_env *env);
void	test_env_hidden(t_env **env);

#endif