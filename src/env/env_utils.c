/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:31:46 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/03 05:09:24 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "structs.h"
#include <limits.h>

/**
 * @brief Increment the SHLVL environment variable
 * 
 * - Reads the current SHLVL, increments it by 1 and writes back
 * - If SHLVL doesn't exist, it initializes it to 1
 * 
 * @param env_list Pointer to list head pointer
 */
void	handle_shlvl(t_env **env_list)
{
	char	*shlvl_str;
	int		shlvl_num;
	char	*new_shlvl;

	shlvl_str = get_env_value(*env_list, "SHLVL");
	if (shlvl_str)
	{
		shlvl_num = ft_atoi(shlvl_str);
		shlvl_num++;
	}
	else
		shlvl_num = 1;
	new_shlvl = ft_itoa(shlvl_num);
	set_env_value(env_list, "SHLVL", new_shlvl);
	free(new_shlvl);
}

/**
 * @brief Ensure PWD exists in the environment
 * 
 * If it's not already set, it reads the current working directory via getcwd()
 * and creates the variable
 * 
 * @param env_list Pointer to list head pointer
 */
void	ensure_pwd(t_env **env_list)
{
	char	*pwd;
	char	cwd[PATH_MAX];

	pwd = get_env_value(*env_list, "PWD");
	if (!pwd)
	{
		if (getcwd(cwd, PATH_MAX))
			set_env_value(env_list, "PWD", cwd);
	}
}

/**
 * @brief Free all nodes in the env linked list
 * 
 * @param env_list Pointer to list head pointer
 */
void	free_env_list(t_env **env_list)
{
	t_env	*current;
	t_env	*next;

	current = *env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

/**
 * @brief Count the number of nodes in the environment list
 * 
 * @param env_list List head
 * @return Number of nodes
 */
int	env_list_size(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}
