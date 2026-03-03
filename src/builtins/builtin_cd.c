/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:22:28 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/02 07:17:09 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get the HOME value from environment
 * 
 * @param env Environment list
 * @return HOME value or NULL with error message
 */
static char	*get_home_path(t_env *env)
{
	char	*path;

	path = get_env_value(env, "HOME");
	if (!path)
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
	return (path);
}

/**
 * @brief Get the target path for cd
 * 
 * Handles no argument or "--" (HOME), "-" (OLDPWD and printed),
 * "-- path" (path) and too many arguments (sets err = 1)
 * 
 * @param args Arguments array
 * @param env Environment list for HOME and OLDPWD lookup
 * @param err Set to 1 if too many arguments and 0 otherwise
 * @return Target path or NULL on error
 */
static char	*get_cd_path(char **args, t_env *env, int *err)
{
	char	*path;

	*err = 0;
	if (!args[1] || ft_strncmp(args[1], "--", 3) == 0)
	{
		if (args[1] && args[2] && args[3])
			return (*err = 1, NULL);
		if (args[1] && args[2])
			return (args[2]);
		return (get_home_path(env));
	}
	if (args[2])
		return (*err = 1, NULL);
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
 * @brief Update OLDPWD and PWD environment variables after cd
 * 
 * @param env Pointer to env list head
 * @param old_pwd Previous working directory (OLDPWD)
 */
static void	update_pwd_vars(t_env **env, char *old_pwd)
{
	char	cwd[PATH_MAX];

	if (old_pwd && old_pwd[0])
		set_env_value(env, "OLDPWD", old_pwd);
	if (getcwd(cwd, PATH_MAX))
		set_env_value(env, "PWD", cwd);
}

/**
 * @brief Implement the `cd` command
 * 
 * Changes the working directory and supports:
 * - no args (HOME)
 * - "-" (OLDPWD)
 * - "--" separator
 * - Error on too many arguments
 * 
 * @param args NULL terminated argument array with arg[0] = "cd"
 * @param env Pointer to env list head
 * @return 0 on success, 1 on error
 */
int	builtin_cd(char **args, t_env **env)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	int		too_many;

	if (!getcwd(old_pwd, PATH_MAX))
		old_pwd[0] = '\0';
	path = get_cd_path(args, *env, &too_many);
	if (too_many)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(path);
		return (1);
	}
	update_pwd_vars(env, old_pwd);
	return (0);
}
