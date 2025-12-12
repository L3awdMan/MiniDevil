/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:07:41 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/12 06:13:16 by baelgadi         ###   ########.fr       */
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

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////// TEMPORARY
//////////////////////////////////////////////////

#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"


void test_pwd(void)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       PWD BUILTIN TEST                 ║\n");
	printf("╚════════════════════════════════════════╝\n");

	printf(YELLOW "\n Test 1 (running in current directory)" RESET);
	printf("\nExpected: %s\n", getcwd(NULL, 0));
	printf("\nGot: ");
	fflush(stdout);
	builtin_pwd();

	printf("\n------------------\n");

	printf(YELLOW "\n Test 2 (running after \"cd /tmp && pwd\")" RESET);
	if (chdir("/tmp") == 0)
	{
		printf("\nExpceted: /tmp\n");
		printf("\nGot: ");
		fflush(stdout);
		builtin_pwd();
	}
	else
		perror("tester error");
}
