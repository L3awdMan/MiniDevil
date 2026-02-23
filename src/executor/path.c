/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 00:32:43 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/23 22:20:23 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_direct_path(char *cmd)
{
	if (!cmd || !*cmd)
		return (0);
	if (ft_strchr(cmd, '/'))
		return (1);
	return (0);
}

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
