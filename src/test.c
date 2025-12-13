/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 06:14:09 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 06:18:58 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define YELLOW "\x1b[33m"
#define GRAY "\x1b[90m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
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

////////////////////////////////////// //////////////////////////////////////
//////////////////////////////////////
////////////////////////////////////// //////////////////////////////////////

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

////////////////////////////////////// //////////////////////////////////////
//////////////////////////////////////
////////////////////////////////////// //////////////////////////////////////

void	test_env(t_env *env)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       ENV BUILTIN TEST                 ║\n");
	printf("╚════════════════════════════════════════╝\n");

	printf(YELLOW "\nFull environment output\n" RESET);
	printf("Printing all variables with values...\n\n" GRAY);
	fflush(stdout);
	
	builtin_env(env);
	printf(RESET);
}

static void	add_hidden_var(t_env **head, char *key)
{
	t_env *new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = NULL;

	new_node->next = *head;
	if (*head)
		(*head)->prev = new_node;
	*head = new_node;

	printf(YELLOW "Simulated export %s (value is NULL)\n" RESET, key);
}

void	test_env_hidden(t_env **env)
{
	// normal var
	set_env_value(env, "VISIBLE_VAR", "hamid");
	printf(YELLOW "\n\nAdded export VISBILE_VAR=hamid\n" RESET);

	// hidden var
	add_hidden_var(env, "HIDDEN_VAR");

	printf("Expected: I should see VISIBLE_VAR\n");
	printf("Expected: I should NOT see HIDDEN_VAR\n\n\n" GRAY);
	fflush(stdout);

	builtin_env(*env);
	printf(RESET);
}

////////////////////////////////////// //////////////////////////////////////
//////////////////////////////////////
////////////////////////////////////// //////////////////////////////////////

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

////////////////////////////////////// //////////////////////////////////////
//////////////////////////////////////
////////////////////////////////////// //////////////////////////////////////

static void	check_unset(t_env *env, char *key)
{
	if (get_env_value(env, key) == NULL)
		printf(GREEN "✅ %s is unset (not found)\n" RESET, key);
	else
		printf(RED "⚠️ %s still exists (unset failed)\n" RESET, key);
}

void	test_unset(t_env **env)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       UNSET BUILTIN TEST               ║\n");
	printf("╚════════════════════════════════════════╝\n");

	set_env_value(env, "VAR", "isbullshit");
	set_env_value(env, "VAR1", "one");
	set_env_value(env, "VAR2", "two");
	set_env_value(env, "VAR3", "three");
	set_env_value(env, "PATH", "/usr/bin");
	printf(GRAY "Added VAR, VAR1, VAR2, VAR3, PATH\n" RESET);

	printf(YELLOW "\nTest 1: unset VAR\n" RESET);
	fflush(stdout);
	char *args1[] = {"unset", "VAR", NULL};
	builtin_unset(args1, env);
	check_unset(*env, "VAR");


	printf(YELLOW "\nTest 2: unset VAR1 VAR2 VAR3\n" RESET);
	fflush(stdout);
	char *args2[] = {"unset", "VAR1", "VAR2", "VAR3", NULL};
	builtin_unset(args2, env);
	check_unset(*env, "VAR1");
	check_unset(*env, "VAR2");
	check_unset(*env, "VAR3");


	printf(YELLOW "\nTest 3: unset NONEXISTENT\n" RESET);
	char *args3[] = {"unset", "NONEXISTENT", NULL};
	int status3 = builtin_unset(args3, env);
	if (status3 == 0)
		printf(GREEN "✅ Returned 0 (no error)\n" RESET);
	else
		printf(RED "⚠️ Returned %d (expected 0)\n" RESET, status3);


	printf(YELLOW "\nTest 4: unset PATH\n" RESET);
	char *args4[] = {"unset", "PATH", NULL};
	builtin_unset(args4, env);
	check_unset(*env, "PATH");


	printf(YELLOW "\nTest 5: unset 1_INVALID\n" RESET);
	char *args5[] = {"unset", "123_INVALID", NULL};
	printf(GRAY "(Expecting error message)\n" RESET);
	fflush(stdout);
	int status5 = builtin_unset(args5, env);
	if (status5 == 1)
		printf(GREEN "✅ returned 1 (correctly flagged as invalid)\n" RESET);
	else
		printf(RED "⚠️ Returned %d (expected 1)\n" RESET, status5);
}

////////////////////////////////////// //////////////////////////////////////
//////////////////////////////////////
////////////////////////////////////// //////////////////////////////////////

void test_pwd(void)
{
	char	*cwd;
	char	*original_dir;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       PWD BUILTIN TEST                 ║\n");
	printf("╚════════════════════════════════════════╝\n");

	original_dir = getcwd(NULL, 0);
	printf(YELLOW "\n Test 1 (running in current directory)" RESET);

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("\nExpected: %s\n", cwd);
		free(cwd);
	}
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

	if (original_dir)
	{
		chdir(original_dir);
		free(original_dir);
	}
}

////////////////////////////////////// //////////////////////////////////////
//////////////////////////////////////
////////////////////////////////////// //////////////////////////////////////

static void	check_var(t_env *env, char *key, char *expected)
{
	char	*val;

	val = get_env_value(env, key);

	if (!val && !expected)
		printf(GREEN "✅ %s is unset/hidden as expected\n" RESET, key);
	else if (!val)
		printf(RED "⚠️ %s is not found (expected %s)\n" RESET, key, expected);
	else if (expected && ft_strncmp(val, expected, -1) == 0)
		printf(GREEN "✅ %s = %s\n" RESET, key, val);
	else
		printf(RED "⚠️ %s = %s (expected %s)\n" RESET, key, val, expected);
}

void	test_export(t_env **env)
{
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       EXPORT BUILTIN TEST              ║\n");
	printf("╚════════════════════════════════════════╝\n");

	printf(YELLOW "\nTest 1: export (no args)\n" RESET);
	printf(GRAY "should print sorted list with 'declare -x'\n\n" RESET);
	fflush(stdout);
	char *args1[] = {"export", NULL};
	builtin_export(args1, env);

	printf(YELLOW "\nTest 2: TEST_VAR=hamid\n" RESET);
	fflush(stdout);
	char *args2[] = {"export", "TEST_VAR=hamid", NULL};
	builtin_export(args2, env);
	check_var(*env, "TEST_VAR", "hamid");

	printf(YELLOW "\nTest 3: VAR1=one VAR2=two\n" RESET);
	fflush(stdout);
	char *args3[] = {"export", "VAR1=one", "VAR2=two", NULL};
	builtin_export(args3, env);
	check_var(*env, "VAR1", "one");
	check_var(*env, "VAR2", "two");

	printf(YELLOW "\nTest 4: 123=invalid (Should print error)\n" RESET);
	fflush(stdout);
	char *args4[] = {"export", "123=invalid", NULL};
	int status4 = builtin_export(args4, env);
	if (status4 == 1)
		printf(GREEN "✅ Returned 1 (error)\n" RESET);
	else
		printf(RED "⚠️ Returned $d (expected 1)\n" RESET);
	// verifying if it was added
	if (get_env_value(*env, "123") == NULL)
		printf(GREEN "✅ Variable 123 was not added\n" RESET);
	else
		printf(RED "⚠️ Variable 123 was added\n" RESET);

	printf(YELLOW "\nTest 5: VAR+=append\n" RESET);
	printf(GRAY "export APPEND_VAR=ham\n" RESET);
	fflush(stdout);
	set_env_value(env, "APPEND_VAR", "ham");
	printf(GRAY "export APPEND_VAR+=_id\n" RESET);
	fflush(stdout);
	char *args5[] = {"export", "APPEND_VAR+=_id", NULL};
	builtin_export(args5, env);
	// verifying now
	char *val = get_env_value(*env, "APPEND_VAR");
	if (val && ft_strncmp(val, "ham_id", -1) == 0)
		printf(GREEN "✅ %s (appended correctly)\n" RESET, val);
	else
		printf(RED "⚠️ %s (expected ham_id)\n" RESET, val);
}

////////////////////////////////////// //////////////////////////////////////
//////////////////////////////////////
////////////////////////////////////// //////////////////////////////////////

void	test_heredoc(t_env *env)
{
	int		fd;
	char	buffer[1024];
	int		bytes;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       HEREDOC TEST                     ║\n");
	printf("╚════════════════════════════════════════╝\n");

	printf(YELLOW "\nTest 1: basic heredoc (delimiter is EOF)\n" RESET);
	fflush(stdout);
	fd = handle_heredoc("EOF", env);

	if (fd >= 0)
	{
		printf(GREEN "\n✅ Heredoc returned, fd = %d\n" RESET, fd);
		printf(GRAY "Content read from heredoc:\n" RESET);
		fflush(stdout);
		while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0)
			{
				buffer[bytes] = '\0';
				printf("%s", buffer);
			}
		printf("\n");
		fflush(stdout);
		close(fd);
	}
	else
		printf(RED "⚠️ heredoc failed\n" RESET);

/////////////////////////

	printf(YELLOW "\nTest 2: heredoc with expansion (type $USER then EOF)\n" RESET);
	fflush(stdout);
	fd = handle_heredoc("EOF", env);
	if (fd >= 0)
	{
		printf(GREEN "\n ✅ Heredoc returned with fd = %d\n" RESET, fd);
		printf(GRAY "Content (should expand):\n" RESET);
		fflush(stdout);
		while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0)
			{
				buffer[bytes] = '\0';
				printf("%s", buffer);
			}
		printf("\n");
		fflush(stdout);
		close(fd);
	}
	else
		printf(RED "⚠️ heredoc failed\n" RESET);

/////////////////////////

	printf(YELLOW "\nTest 3: quoted delimiter (type $USER then EOF)\n"RESET);
	printf(GRAY "Delimiter is 'EOF' (no expansion expected)\n" RESET);
	fflush(stdout);
	fd = handle_heredoc("'EOF'", env);
	if (fd >= 0)
	{
		printf(GREEN "\n✅ Heredoc returned with fd = %d\n" RESET, fd);
		printf(GRAY "Content (should NOT expand):\n" RESET);
		fflush(stdout);
		while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes] = '\0';
			printf("%s", buffer);
		}
	printf("\n");
	close(fd);
	}
	else
		printf(RED "⚠️ heredoc failed\n" RESET);
}

////////////////////////////////////// //////////////////////////////////////
//////////////////////////////////////
////////////////////////////////////// //////////////////////////////////////


void	test_path_finder(t_env *env)
{
	char	*path;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║       PATH FINDER TEST                 ║\n");
	printf("╚════════════════════════════════════════╝\n");

	char *debug_path = get_env_value(env, "PATH");
	if (debug_path)
		printf(GRAY "!DEBUG [%s]\n" RESET, debug_path);
	else
		printf(RED "Problem with env init\n");


	printf(YELLOW "\nTest 1: \"ls\"\n" RESET);
	path = find_cmd_path("ls", env);
	if (path)
		printf(GREEN "✅ Found %s\n" RESET, path);
	else
		printf(RED "⚠️ Not found\n" RESET);
	free(path);

	printf(YELLOW "\nTest 2: \"cat\"\n" RESET);
	path = find_cmd_path("cat", env);
	if (path)
		printf(GREEN "✅ Found %s\n" RESET, path);
	else
		printf(RED "⚠️ Not found\n" RESET);
	free(path);

	printf(YELLOW "\nTest 3: \"NONEXISTENT\"\n" RESET);
	path = find_cmd_path("NONEXISTENT", env);
	if (!path)
		printf(GREEN "Correctly returned NULL\n" RESET);
	else
	{
		printf(RED "⚠️ got %s (expected NULL)\n" RESET, path);
		free(path);
	}

	printf(YELLOW "\nTest 4: absolute path \"/bin/ls\"\n" RESET);
	path = find_cmd_path("/bin/ls", env);
	if (path && ft_strncmp(path, "/bin/ls", -1) == 0)
		printf(GREEN "✅ Found %s\n" RESET, path);
	else
		printf(RED "⚠️ Fail\n" RESET);
	free(path);

	printf(YELLOW "\nTest 5: invalid absolute path \"/nonexistent/ls\"\n" RESET);
	path = find_cmd_path("/nonexistent/ls", env);
	if (!path)
		printf(GREEN "✅ Correctly returned NULL\n" RESET);
	else
	{
		printf(RED "⚠️ got %s (expected NULL)\n" RESET, path);
		free(path);
	}

	printf(YELLOW "\nTest 6: empty \"\"\n" RESET);
	path = find_cmd_path("", env);
	if (!path)
		printf(GREEN "✅ Correctly returned NULL\n" RESET);
	else
	{
		printf(RED "⚠️ got %s (expected NULL)\n" RESET, path);
		free(path);
	}
}

////////////////////////////////////// //////////////////////////////////////
//////////////////////////////////////
////////////////////////////////////// //////////////////////////////////////
