/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 05:00:41 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/12 06:13:03 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if an argument is a valid -n flag
 * 
 * Valid flags must start with '-' followed by 1 or more 'n' characters
 * - Examples: "-n", "-nn", "-nnn" are valid while "-na" or "-" are not
 * @return 1 if the arg is a valid -n flag, 0 if not
 */
static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Implement the echo builtin command
 * 
 * This mimics the behavior of the bash echo command
 * 1. Skip all initial arguments that match the -n flag pattern
 * 2. If some flags were found, disable the newline
 * 3. Print the rest of the arguments with 1 space in between
 * 4. PRint a newline if the flag was not unset
 * 
 * @param args Null terminated array of args (arg[0] should be "echo")
 * @return Always 0 (success)
 */
int builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && is_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
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

static void	run_compare(char **args, char *expected_txt, int expected_nl)
{
	int	i;

	i = 0;
	while (args[i])
	{
		printf(YELLOW "%s " RESET, args[i]);
		i++;
	}
	printf("\n");

	printf("EXP: |%s", expected_txt);
	if (expected_nl)
		printf("\n");
	printf("|\n");

	printf("GOT: |");
	fflush(stdout);
	builtin_echo(args);
	printf("|\n");
	printf("---------------------\n");
}

void test_echo(void)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       ECHO BUILTIN TEST                ║\n");
	printf("╚════════════════════════════════════════╝\n");

	char *test1[] = {"echo", "hamid", NULL};
	run_compare(test1, "hamid", 1);

	char *test2[] = {"echo", "hello", "world", NULL};
	run_compare(test2, "hello world", 1);

	char *test3[] = {"echo", "-n", "hello", NULL};
	run_compare(test3, "hello", 0);

	char *test4[] = {"echo", "-n", "-n", "-n", "hello", NULL};
	run_compare(test4, "hello", 0);

	char *test5[] = {"echo", "-nnnnn", "hello", NULL};
	run_compare(test5, "hello", 0);

	char *test6[] = {"echo", "-n", "-abc", "hello", NULL};
	run_compare(test6, "-abc hello", 0);
}
