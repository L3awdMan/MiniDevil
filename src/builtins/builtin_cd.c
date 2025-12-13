/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:22:28 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 06:40:34 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get the target path for the cd command
 * Handles special cases
 * - No arguments => default to HOME
 * - "-" => default to OLDPWD (and print the path)
 * @param args Command arguments
 * @param env Pointer to the environment list
 * @return The path to change to or NULL on error
 */
static char	*get_cd_path(char **args, t_env *env)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value(env, "HOME");
		if (!path)
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		return (path);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_value(env, "OLDPWD");
		if (!path)
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		else
			ft_putendl_fd(path, STDOUT_FILENO);
		return (path);
	}
	return (args[1]);
}

/**
 * @brief Update PWD and OLDPWD environment variables
 * 
 * @param env Double pointer to the environment list (needed by set_env_value())
 * @param old_pwd The previous working directory
 * @return always 0 (success)
 */
static int	update_pwd_vars(t_env **env, char *old_pwd)
{
	char	cwd[PATH_MAX];

	if (old_pwd && old_pwd[0])
		set_env_value(env, "OLDPWD", old_pwd);
	if (getcwd(cwd, PATH_MAX))
		set_env_value(env, "PWD", cwd);
	return (0);
}

/**
 * @brief Implement the change directory builtin command
 * 
 * 1. Save the current directory
 * 2. Determine the target path
 * 3. Change directory using chdir()
 * 4. Update variables PWD and OLDPWD in the environment
 * @param args Arguments array (arg[1] being the path)
 * @param env Double pointer to the environment list
 * @return 0 on success and 1 on fail
 */
int	builtin_cd(char **args, t_env **env)
{
	char	*path;
	char	old_pwd[PATH_MAX];

	if (!getcwd(old_pwd, PATH_MAX))
		old_pwd[0] = '\0';
	path = get_cd_path(args, *env);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(args[1]);
		return (1);
	}
	update_pwd_vars(env, old_pwd);
	return (0);
}
