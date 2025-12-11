/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:03:44 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/11 07:15:06 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int builtin_echo(char **args);

int	builtin_pwd(void);

int	builtin_cd(char **args, t_env **env);


///// TEMPORARY

void test_echo(void);
void test_pwd(void);
void test_cd(t_env **env);


#endif