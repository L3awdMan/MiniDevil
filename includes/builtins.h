/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:03:44 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/11 22:33:20 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int builtin_echo(char **args);

int	builtin_pwd(void);

int	builtin_cd(char **args, t_env **env);

int	builtin_exit(char **args, int last_status);


///// TEMPORARY

void	test_echo(void);
void	test_pwd(void);
void	test_cd(t_env **env);
void	test_exit(void);


#endif