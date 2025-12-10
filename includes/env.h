/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:44:42 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 02:16:33 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

/**
 * @file env.h
 * @brief Environment variable management prototypes
 *
 * Handles the shell's environment using a doubly linked list
 * Supports operations needed for export and unset commands + variable expansion
 */

# include "minishell.h"
# include "structs.h"

//==================================================
//================ INITIALIZATION ==================
//==================================================

/**
 * @brief Initialize environment from envp
 *
 * Create a doubly linked list from the envp array passed to main
 *
 * @param envp Environment array (from main)
 * @return t_env* Head of environment list
 */
t_env	*init_env(char **envp);

/**
 * @brief Create a single environment node
 *
 * Parse "KEY=value" string into separate key and value
 *
 * @param env_string String in format "KEY=value"
 * @return New environment node
 */
t_env	*create_env_node(char *env_string);

/**
 * @brief Add node to the end of environment list
 *
 * @param head Pointer to list head
 * @param new_node Node to add
 */
void	add_env_node(t_env **head, t_env *new_node);

/**
 * @brief Get value of environment variable
 *
 * @param env_list Environment list
 * @param key Variable name to find
 * @return VAlue string or NULL if nothing is found
 *
 * @note Returns a pointer to internal string (shouldn't free)
 */
char	*get_env_value(t_env *env_list, char *key);

/**
 * @brief Set or update the environment variable
 *
 * @param env_list Pointer to the environment list head
 * @param key Variable name
 * @param value New value
 * @return 1 on success and 0 on failure
 */
int		set_env_value(t_env **env_list, char *key, char *value);

//==================================================
//=================== UTILITIES ====================
//==================================================

/**
 * @brief Handle shell level (not needed and doesn't create any bug if absent)
 *
 * @param env_list
 */
void	handle_shlvl(t_env **env_list);

/**
 * @brief Ensures PWD is set
 *
 * Sets PWD to the current directory if not already set
 *
 * @param env_list Pointer to the environment list head
 */
void	ensure_pwd(t_env **env_list);

/**
 * @brief Free the entire environment list
 *
 * @param env_list Pointer to the environment list head
 */
void	free_env_list(t_env **env_list);

/**
 * @brief Count nodes in the environment list
 *
 * @param env_list Environment list head
 * @return Number of environment variables
 */
int		env_list_size(t_env *env_list);

#endif
