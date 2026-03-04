/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 00:32:43 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/04 08:50:41 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "env.h"
#include "libft.h"

/**
 * @brief Check if a command contains a slash (= direct path)
 * 
 * @param cmd Command to check
 * @return 1 if cmd contains `/` and 0 if not
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
 * @brief Join a directory and command name with a `/` in between
 * 
 * @param dir Directory path
 * @param cmd Command name
 * @return Newly allocated "dir/cmd" string or NULL
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
 * @brief Search an executable command in a directory
 * 
 * Joins dir and cmd then checks if their result is executable via access(X_OK)
 * 
 * @param dir Directory to search in
 * @param cmd Command name to look for
 * @return Full path if found AND executable or NULL
 */
char	*search_in_dir(char *dir, char *cmd)
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
 * @brief Search for a command in all PATH directories
 * 
 * Splits the PATH variable (by `:`) and tries each directory
 * 
 * @param cmd Command name to search for
 * @param path_var Value of the PATH environment variable
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
 * @brief Resolve a command name into its full executable path
 * 
 * If the command contains `/` it's treated as a direct path. Otherwise it looks
 * up PATH in env and searches each directory until an executable match is found
 * 
 * @param cmd Command name or path to resolve
 * @param env Environment list
 * @return Full path, CWD match if PATH is unset or NULL if not found
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
		return (search_in_dir(".", cmd));
	return (search_in_path(cmd, path_var));
}
