/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:44:42 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/11 02:51:55 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "structs.h"

/* ────────────── env_init.c ──────────────── */

t_env	*init_env(char **envp);

t_env	*create_env_node(char *env_string);

void	add_env_node(t_env **head, t_env *new_node);

/* ────────────── env_operations.c ──────────────── */

char	*get_env_value(t_env *env_list, char *key);

int		set_env_value(t_env **env_list, char *key, char *value);

/* ────────────── env_utils.c ──────────────── */

void	handle_shlvl(t_env **env_list);

void	ensure_pwd(t_env **env_list);

void	free_env_list(t_env **env_list);

int		env_list_size(t_env *env_list);

/* ────────────── env_conversion.c ──────────────── */

char	**env_to_array(t_env *env);

#endif
