/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:44:42 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:40:46 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"
# include "structs.h"

t_env	*init_env(char **envp);

t_env	*create_env_node(char *env_string);

void	add_env_node(t_env **head, t_env *new_node);

char	*get_env_value(t_env *env_list, char *key);

int		set_env_value(t_env **env_list, char *key, char *value);

void	handle_shlvl(t_env **env_list);

void	ensure_pwd(t_env **env_list);

void	free_env_list(t_env **env_list);

int		env_list_size(t_env *env_list);

char	**env_to_array(t_env *env);

#endif
