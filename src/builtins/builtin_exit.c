/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:53:47 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/12 06:18:36 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str) // replace by libft ft_str_is_numeric()
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	count_args(char **args) // replace by ft_arrlen()
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

/**
 * @brief Exit the shell with specific status code
 * 
 * 1. If no arguments, exit with the status of the last executed command
 * 2. If the first arg is not numeric, print an error and exit with status 2
 * (no matter how many arguments there are !!)
 * 3. If the first argument is numeric but there are extra arguments, print
 * "too many arguments" and return 1 WITHOUT exiting
 * 4. If valid, convert the first argument to an 8 bit status exit code (0-255)
 * @param args Null terminated array (args[0] is "exit")
 * @param last_status The exit code of the last executed command
 * @return 1 if too many arguments, otherwise doesn't return but exits
 */
int	builtin_exit(char **args, int last_status)
{
	int	argc;
	int	exit_code;

	ft_putstr_fd("exit\n", 1);
	argc = count_args(args);
	if (argc == 1)
		exit(last_status);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit (2);
	}
	if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	exit_code = (unsigned char)ft_atoi(args[1]);
	exit(exit_code);
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
#define GRAY "\x1b[90m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

// 💀💀💀💀💀
static void run_test(char **args, int last_status, int expected, int should_exit)
{
	pid_t	pid;
	int		status;
	int		exit_code;
	int		ret;

	pid = fork();
	if (pid == 0)
	{ //-------------- CHILD PROCESS
		ret = builtin_exit(args, last_status);
		if (ret == 1)
			exit(133); // (if it returned it will exit with 133, check later)
		exit(0); // I shouldnt reach here
	}
	waitpid(pid, &status, 0);

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);

		// Expecting the function to exit the shell
		if (should_exit)
		{
			if (exit_code == expected)
				printf (GREEN "✅ exited with %d\n" RESET, exit_code);
			else
				printf (RED "⚠️ exited with %d (expected %d)\n" RESET, exit_code, expected);
		}

		// Expecting the function to not exit
		else
		{
			if (exit_code == 133)
				printf(GREEN "✅ function returned\n" RESET);
			else
				printf(RED "⚠️ exited with %d (should have returned)\n" RESET, exit_code);
		}
	}
}

void test_exit(void)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       EXIT BUILTIN TEST                ║\n");
	printf("╚════════════════════════════════════════╝\n");

		printf(YELLOW "\n Test 1 (no args)\n" RESET);
		fflush(stdout);
		char *args1[] = {"exit", NULL};

		printf(GRAY "last_status=0\n" RESET);
		fflush(stdout);
		run_test(args1, 0, 0, 1);

		printf(GRAY "last_status=42\n" RESET);
		fflush(stdout);
		run_test(args1, 42, 42, 1);

		printf(YELLOW "\n Test 2 (exit 0)\n" RESET);
		fflush(stdout);
		char *args2[] = {"exit", "0", NULL};
		run_test(args2, 123, 0, 1);

		printf(YELLOW "\n Test 3 (exit 42)\n" RESET);
		fflush(stdout);
		char *args3[] = {"exit", "42", NULL};
		run_test(args3, 0, 42, 1);

		printf(YELLOW "\n Test 4 (exit 256 -> wrap to 0)\n" RESET);
		fflush(stdout);
		char *args4[] = {"exit", "256", NULL};
		run_test(args4, 0, 0, 1);

		printf(YELLOW "\n Test 5 (exit 257 -> wrap to 1)\n" RESET);
		fflush(stdout);
		char *args5[] = {"exit", "257", NULL};
		run_test(args5, 0, 1, 1);

		printf(YELLOW "\n Test 6 (exit -1 -> wrap to 255)\n" RESET);
		fflush(stdout);
		char *args6[] = {"exit", "-1", NULL};
		run_test(args6, 0, 255, 1);

		printf(YELLOW "\n Test 7 (exit abc -> exit 2 + error)\n" RESET);
		fflush(stdout);
		char *args7[] = {"exit", "abc", NULL};
		run_test(args7, 0, 2, 1);

		printf(YELLOW "\n Test 8 (exit 1 2 -> too many args = NO EXIT)\n" RESET);
		fflush(stdout);
		char *args8[] = {"exit", "1", "2", NULL};
		run_test(args8, 0, 0, 0);
}
