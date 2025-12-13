/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 00:32:43 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 06:38:01 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if a command is a valid path (absolute or relative)
 * 
 * In Bash if a command contains any slash the path lookup is skipped
 * (example bin/ls or ./script or ./bin/ls)
 * @param cmd The command string
 * @return 1 if it's a path, 0 if it's just a cmd name
 */
static int	is_direct_path(char *cmd)
{
	if (!cmd || !*cmd)
		return (0);
	if (ft_strchr(cmd, '/'))
		return (1);
	return (0);
}

/**
 * @brief Join the directory path with the command name
 * 
 * @param dir The directory gotten from PATH
 * @param cmd The command name
 * @return A newly allocated full path or NULL on fail
 */
static char	*join_path(char *dir, char *cmd)
{
	char	*path_with_slash;
	char	*full_path;

	path_with_slash = ft_strjoin(dir, "/");
	if (!path_with_slash)
		return (NULL);
	full_path = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	return (full_path);
}

/**
 * @brief Search for the command in a single directory
 * 
 * @param dir The directory where to search
 * @param cmd The command name
 * @return Full path if found AND executable, or NULL if not
 */
static char	*search_in_dir(char *dir, char *cmd)
{
	char	*full_path;

	if (!dir || !*dir)
		return (NULL);
	full_path = join_path(dir, cmd);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

/**
 * @brief Search for the executable in PATH directories
 * 
 * Split PATH by : and search each directory
 * @param cmd The command name
 * @param path_var The PATH env variable value
 * @return Full path to the executable or NULL if not found
 */
static char	*search_in_path(char *cmd, char *path_var)
{
	char	**dirs;
	char	*result;
	int		i;

	i = 0;
	result = NULL;
	dirs = ft_split(path_var, ':');
	if (!dirs)
		return (NULL);
	while (dirs[i] && !result)
	{
		result = search_in_dir(dirs[i], cmd);
		i++;
	}
	ft_free_strarray(dirs);
	return (result);
}

/**
 * @brief Find the full path of a command
 * 
 * - If a command is empty return NULL
 * - If a command is a direct path (/ ./ ../) validate and return copy
 * - If PATH is not set return NULL
 * - Search each PATH directory for the command one by one
 * @param cmd The command to find
 * @param env The environment list
 * @warning This only checks if the command exists. If it's not executable then
 * it needs to be handled directly in the executor
 * @return Newly allocated string (full path) or NULL if not found
 */
char	*find_cmd_path(char *cmd, t_env *env)
{
	char	*path_var;

	if (!cmd || !*cmd)
		return (NULL);
	if (is_direct_path(cmd))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_var = get_env_value(env, "PATH");
	if (!path_var)
		return (NULL);
	return (search_in_path(cmd, path_var));
}
