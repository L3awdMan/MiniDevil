/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:22:28 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/12 06:13:10 by baelgadi         ###   ########.fr       */
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

	if (!getcwd(old_pwd, PATH_MAX)) // save current pwd for later
		old_pwd[0] = '\0'; // unlikely error but you never know

	path = get_cd_path(args, *env); // resolve path
	if (!path)
		return(1);
	if (chdir(path) == -1) // change directory
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(args[1]);
		return (1);
	}
	update_pwd_vars(env, old_pwd);
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
#define GRAY "\x1b[90m"
#define RESET "\x1b[0m"


void test_cd(t_env **env)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       CD BUILTIN TEST                  ║\n");
	printf("╚════════════════════════════════════════╝\n");

	char*cwd;
	char *new_pwd;

	///// TEST 1 (absolute path)
	printf(YELLOW "\nTest 1 (cd /tmp)" RESET);
	char *args1[] = {"cd", "/tmp", NULL};

	printf(GRAY "\nCalling builtin_cd with /tmp" RESET);
	builtin_cd(args1, env);

	new_pwd = getcwd(NULL, 0);
	printf("\nExpected PWD: /tmp");
	printf("\nGot PWD: %s", new_pwd);
	printf("\nEnv Var PWD: %s\n", get_env_value(*env, "PWD"));
	free(new_pwd);

///////////////////////////////////////////

	///// TEST 2 (PARENT DIRECTORY)
	printf(YELLOW "\nTest 2 (cd ..)" RESET);
	char *args_parent[] = {"cd", "..", NULL};

	printf(GRAY "\nCalling builtin_cd with .." RESET);
	builtin_cd(args_parent, env);

	new_pwd = getcwd(NULL, 0);
	printf("\nExpected PWD: /");
	printf("\nGot PWD: %s", new_pwd);
	printf("\nEnv Var PWD: %s\n", get_env_value(*env, "PWD"));
	free(new_pwd);

///////////////////////////////////////////

	///// TEST 3 (-)
	printf(YELLOW "\nTest 3 (previous)" RESET);
	char *args2[] = {"cd", "-", NULL};

	printf(GRAY "\nCalling builtin_cd with -" RESET);
	printf("\n(Expected path): ");
	fflush(stdout);
	builtin_cd(args2, env);

	printf("\nEnv Var PWD: %s\n", get_env_value(*env, "PWD"));

///////////////////////////////////////////

	///// TEST 4 (NO ARGS)
	printf(YELLOW "\nTest 4 (cd [HOME])" RESET);
	char *args3[] = {"cd", NULL};
	char *home = get_env_value(*env, "HOME");

	printf(GRAY "\nCalling builtin_cd with no args" RESET);
	builtin_cd(args3, env);

	new_pwd = getcwd(NULL, 0);
	printf("\nExpected PWD: %s", home);
	printf("\nGOt PWD: %s\n\n", new_pwd);
	free(new_pwd);

///////////////////////////////////////////

	///// TEST 5 (Non existent)
	printf(YELLOW "\nTest 5 (cd /nonexistent)" RESET);
	char *args4[] = {"cd", "/nonexistent", NULL};

	printf(GRAY "\nCalling builtin_cd with bad path\n" RESET);
	fflush(stdout);
	int ret = builtin_cd(args4, env);

	printf("Return value: %d (Expected: 1)\n", ret);

	cwd = getcwd(NULL, 0);
	printf("Current PWD: %s\n", cwd);
	free(cwd);
}
