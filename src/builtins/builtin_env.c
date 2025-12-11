/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:53:19 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/11 23:31:25 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implement the env builtin command
 * 
 * Iterates through the env linked list and prints "KEY=VALUE" for every
 * variable that has a valid value (non NULL)
 * Variables with null values (created by export without value) are ignored
 * @param env Pointer to the head of the environment list
 * @return Always 0 (success)
 */
int	builtin_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(current->value, STDOUT_FILENO);
		}
		current = current->next;
	}
	return (0);
}

////////////////. TEMPORARY

#define YELLOW "\x1b[33m"
#define GRAY "\x1b[90m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

void	test_env(t_env *env)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       ENV BUILTIN TEST                 ║\n");
	printf("╚════════════════════════════════════════╝\n");

	printf(YELLOW "\nFull environment output\n" RESET);
	printf("Printing all variables with values...\n\n" GRAY);
	fflush(stdout);
	
	builtin_env(env);
	printf(RESET);
}

static void	add_hidden_var(t_env **head, char *key)
{
	t_env *new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = NULL;

	new_node->next = *head;
	if (*head)
		(*head)->prev = new_node;
	*head = new_node;

	printf(YELLOW "Simulated export %s (value is NULL)\n" RESET, key);
}

void	test_env_hidden(t_env **env)
{
	// normal var
	set_env_value(env, "VISIBLE_VAR", "hamid");
	printf(YELLOW "\n\nAdded export VISBILE_VAR=hamid\n" RESET);

	// hidden var
	add_hidden_var(env, "HIDDEN_VAR");

	printf("Expected: I should see VISIBLE_VAR\n");
	printf("Expected: I should NOT see HIDDEN_VAR\n\n\n" GRAY);
	fflush(stdout);

	builtin_env(*env);
	printf(RESET);
}