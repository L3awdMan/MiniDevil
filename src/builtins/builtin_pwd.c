/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:07:41 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 06:17:58 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implement the printing working directory builtin command
 * 
 * This retrieves the current working directory using getcwd()
 * and prints it to the standard output, followed by a newline
 * 1. Allocate a buffer of size PATH_MAX (often 4096) on the stack
 * 2. Call getcwd() to store the current path in the buffer
 * 3. If getcwd() fails then print an error message and return 1
 * 4. If successful print the buffer with nl using ft_putendl_fd() from libft
 * 
 * @return 0 on success, 1 on failure
 */
int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	return (0);
}
