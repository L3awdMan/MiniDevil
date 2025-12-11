/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:31:46 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/11 00:00:26 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "structs.h"
#include <limits.h>

//======================== FUNCTION: handle_shlvl ===========================
	//
	// PURPOSE: Increment SHLVL when minishell starts
	//
	// ALGORITHM:
	// 1. Get current SHLVL value: shlvl_str = get_env_value(*env_list, "SHLVL")
	// 2. IF shlvl_str exists (not NULL):
	//    a. Convert to int: shlvl_num = ft_atoi(shlvl_str)
	//    b. Increment: shlvl_num++
	// 3. ELSE (SHLVL doesn't exist):
	//    a. Set shlvl_num = 1
	// 4. Convert number back to string: new_shlvl = ft_itoa(shlvl_num)
	// 5. Update environment: set_env_value(env_list, "SHLVL", new_shlvl)
	// 6. Free the string: free(new_shlvl)
	//
	// WHY?
	//    Each nested shell should know its depth:
	//    bash (SHLVL=1) → minishell (SHLVL=2) → minishell (SHLVL=3)
	//
	// PARAMETERS:
	//    t_env **env_list - Pointer to environment list
	//
	// RETURN:
	//    void
	//
	// EXAMPLE:
	//    Before: SHLVL=2
	//    After:  SHLVL=3
	//
	// VARIABLES:
	//    char *shlvl_str - Current SHLVL value as string (from environment)
	//    int shlvl_num   - SHLVL as integer (for incrementing)
	//    char *new_shlvl - New SHLVL value as string (to store back)

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

//======================== FUNCTION: ensure_pwd =============================
	//
	// PURPOSE: Make sure PWD exists in environment
	//
	// ALGORITHM:
	// 1. Check if PWD exists: pwd = get_env_value(*env_list, "PWD")
	// 2. IF pwd is NULL (doesn't exist):
	//    a. Get current directory: getcwd(cwd, PATH_MAX)
	//    b. IF getcwd succeeded (returned non-NULL):
	//       - Set PWD: set_env_value(env_list, "PWD", cwd)
	//
	// WHY?
	//    PWD is required by many commands (cd, pwd builtin)
	//    If it's missing, we need to set it to current directory
	//
	// PARAMETERS:
	//    t_env **env_list - Pointer to environment list
	//
	// RETURN:
	//    void
	//
	// EXAMPLE:
	//    Current directory: /home/user/minishell
	// No PWD in environment
	//    → Sets PWD=/home/user/minishell
	//
	// VARIABLES:
	//    char *pwd      - Existing PWD value (NULL if doesn't exist)
	//    char cwd[PATH_MAX] - Buffer to store current working directory

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

//======================== FUNCTION: free_env_list ==========================
	//
	// PURPOSE: Free entire environment list
	//
	// ALGORITHM:
	// 1. Set current = env_list (start at head)
	// 2. WHILE current is not NULL:
	//    a. Save next pointer: next = current->next
	//    b. Free key: free(current->key)
	//    c. Free value: free(current->value)
	//    d. Free node: free(current)
	//    e. Move to next: current = next
	//
	// WHY save next pointer?
	//    Because after we free(current), we can't access current->next!
	//    We must save it BEFORE freeing the node.
	//
	// PARAMETERS:
	//    t_env *env_list - Head of environment list
	//
	// RETURN:
	//    void
	//
	// EXAMPLE:
	//    List: [HOME] <-> [PATH] <-> [USER] <-> NULL
	//    After free_env_list(): All memory freed
	//
	// VARIABLES:
	//    t_env *current - Current node being freed
	//    t_env *next    - Next node (saved before freeing current)

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

//======================== FUNCTION: env_list_size ==========================
	//
	// PURPOSE: Count number of nodes in list
	//
	// ALGORITHM:
	// 1. Initialize count = 0
	// 2. Set current = env_list
	// 3. WHILE current exists:
	//    a. Increment count
	//    b. Move to next: current = current->next
	// 4. Return count
	//
	// PARAMETERS:
	//    t_env *env_list - Head of environment list
	//
	// RETURN:
	//    int - Number of nodes in list
	//
	// EXAMPLE:
	//    List: [HOME] <-> [PATH] <-> [USER] <-> NULL
	//    Returns: 3
	//
	// VARIABLES:
	//    int count      - Counter for nodes
	//    t_env *current - Pointer to traverse list

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
