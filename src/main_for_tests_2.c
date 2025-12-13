/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_for_tests_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:31:50 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/13 05:02:14 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*   main_for_tests.c                                                        */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "env.h"
#include "minishell.h"
#include "parser.h"
#include "structs.h"
#include "token.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>

void	print_tokens(t_token *tokens)
{
	t_token	*curr;
	int		i;

	curr = tokens;
	i = 0;
	while (curr)
	{
		printf("  [%d] type=%d value='%s'\n", i, curr->type, curr->value);
		curr = curr->next;
		i++;
	}
}
void	test_quote_concatenation(void)
{
	t_token	*tokens;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   QUOTE CONCATENATION TESTS            ║\n");
	printf("╚════════════════════════════════════════╝\n");
	printf("\n✓ Test 1: Word with embedded quotes\n");
	printf("Input: echo hami\"d\"\n");
	tokens = tokenize("echo hami\"d\"");
	print_tokens(tokens);
	printf("Expected: [echo] [hamid] (2 tokens)\n");
	if (token_list_size(tokens) == 2)
		printf("✅ Correct token count\n");
	else
		printf("⚠️  Got %d tokens (needs quote merging)\n",
			token_list_size(tokens));
	free_token_list(tokens);
	printf("\n✓ Test 2: Multiple quote sections\n");
	printf("Input: echo \"hello\"world\"test\"\n");
	tokens = tokenize("echo \"hello\"world\"test\"");
	print_tokens(tokens);
	printf("Expected: [echo] [helloworldtest] (2 tokens)\n");
	if (token_list_size(tokens) == 2)
		printf("✅ Correct token count\n");
	else
		printf("⚠️  Got %d tokens (needs quote merging)\n",
			token_list_size(tokens));
	free_token_list(tokens);
	printf("\n✓ Test 3: Quotes at start\n");
	printf("Input: echo \"ha\"mid\n");
	tokens = tokenize("echo \"ha\"mid");
	print_tokens(tokens);
	printf("Expected: [echo] [hamid] (2 tokens)\n");
	if (token_list_size(tokens) == 2)
		printf("✅ Correct token count\n");
	else
		printf("⚠️  Got %d tokens (needs quote merging)\n",
			token_list_size(tokens));
	free_token_list(tokens);
	printf("\n✓ Test 4: Empty quotes concatenation\n");
	printf("Input: echo a\"\"b\n");
	tokens = tokenize("echo a\"\"b");
	print_tokens(tokens);
	printf("Expected: [echo] [ab] (2 tokens)\n");
	if (token_list_size(tokens) == 2)
		printf("✅ Correct token count\n");
	else
		printf("⚠️  Got %d tokens (needs quote merging)\n",
			token_list_size(tokens));
	free_token_list(tokens);
	printf("\n✓ Test 5: Mixed single and double quotes\n");
	printf("Input: echo ha'mi'\"d\"\n");
	tokens = tokenize("echo ha'mi'\"d\"");
	print_tokens(tokens);
	printf("Expected: [echo] [hamid] (2 tokens)\n");
	if (token_list_size(tokens) == 2)
		printf("✅ Correct token count\n");
	else
		printf("⚠️  Got %d tokens (needs quote merging)\n",
			token_list_size(tokens));
	free_token_list(tokens);
}

void	debug_environment(t_env *env)
{
	t_env	*current;
	int		count;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   ENVIRONMENT DEBUG                    ║\n");
	printf("╚════════════════════════════════════════╝\n\n");
	// Count variables
	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	printf("📊 Total environment variables: %d\n\n", count);
	// Check specific variables
	printf("🔍 Checking critical variables:\n");
	printf("   USER  = '%s'\n", get_env_value(env, "USER") ? get_env_value(env,
			"USER") : "(NULL)");
	printf("   HOME  = '%s'\n", get_env_value(env, "HOME") ? get_env_value(env,
			"HOME") : "(NULL)");
	printf("   PATH  = '%s'\n", get_env_value(env, "PATH") ? get_env_value(env,
			"PATH") : "(NULL)");
	printf("   PWD   = '%s'\n", get_env_value(env, "PWD") ? get_env_value(env,
			"PWD") : "(NULL)");
	printf("   SHLVL = '%s'\n", get_env_value(env, "SHLVL") ? get_env_value(env,
			"SHLVL") : "(NULL)");
	// List first 10 variables
	printf("\n📋 First 10 environment variables:\n");
	current = env;
	count = 0;
	while (current && count < 10)
	{
		printf("   [%d] %s = '%s'\n", count, current->key, current->value);
		current = current->next;
		count++;
	}
	printf("\n");
}
void	test_parser(void)
{
	t_token	*tokens;
	t_ast	*tree;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   PARSER TESTS                         ║\n");
	printf("╚════════════════════════════════════════╝\n");
	// Test 1: Simple command
	printf("\n✓ Test 1: Simple command\n");
	tokens = tokenize("echo hello world");
	tree = parse(tokens);
	if (tree && tree->type == NODE_COMMAND && tree->data.cmd.argc == 3)
		printf("✅ Parsed: echo hello world (3 args)\n");
	else
		printf("❌ Failed!\n");
	free_ast(tree);
	free_token_list(tokens);
	// Test 2: Pipeline
	printf("\n✓ Test 2: Pipeline\n");
	tokens = tokenize("cat file.txt | grep test");
	tree = parse(tokens);
	if (tree && tree->type == NODE_PIPE)
		printf("✅ Parsed: cat | grep (pipe node)\n");
	else
		printf("❌ Failed!\n");
	free_ast(tree);
	free_token_list(tokens);
	// Test 3: Multi-pipe
	printf("\n✓ Test 3: Multi-pipeline\n");
	tokens = tokenize("cat | grep | sort");
	tree = parse(tokens);
	if (tree && tree->type == NODE_PIPE)
		printf("✅ Parsed: cat | grep | sort\n");
	else
		printf("❌ Failed!\n");
	free_ast(tree);
	free_token_list(tokens);
}

void	test_ast_quick(void)
{
	t_ast	*cmd;
	char	**args;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   AST QUICK TEST                       ║\n");
	printf("╚════════════════════════════════════════╝\n");
	// Test 1: Create command node
	args = malloc(sizeof(char *) * 3);
	args[0] = ft_strdup("echo");
	args[1] = ft_strdup("hello");
	args[2] = NULL;
	cmd = create_cmd_node(args, 2);
	if (cmd && cmd->type == NODE_COMMAND)
		printf("✅ Command node created: %s %s\n", cmd->data.cmd.args[0],
			cmd->data.cmd.args[1]);
	else
		printf("❌ Command node failed!\n");
	free_ast(cmd);
	printf("✅ AST cleanup works!\n");
}

void	test_tokenizer(void)
{
	t_token	*tokens;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   TOKENIZER TESTS                      ║\n");
	printf("╚════════════════════════════════════════╝\n");
	printf("\n✓ Test 1: Simple command\n");
	printf("Input: 'echo hello'\n");
	tokens = tokenize("echo hello");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 2: Pipe operator\n");
	printf("Input: 'cat file.txt | grep test'\n");
	tokens = tokenize("cat file.txt | grep test");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 3: Input redirection\n");
	printf("Input: 'cat < input.txt'\n");
	tokens = tokenize("cat < input.txt");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 4: Output redirection\n");
	printf("Input: 'echo hello > output.txt'\n");
	tokens = tokenize("echo hello > output.txt");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 5: Heredoc\n");
	printf("Input: 'cat << EOF'\n");
	tokens = tokenize("cat << EOF");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 6: Append\n");
	printf("Input: 'echo test >> file.txt'\n");
	tokens = tokenize("echo test >> file.txt");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 7: Adjacent operators\n");
	printf("Input: 'cat<<EOF>>output'\n");
	tokens = tokenize("cat<<EOF>>output");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 8: Complex pipeline\n");
	printf("Input: 'cat < in.txt | grep test | sort > out.txt'\n");
	tokens = tokenize("cat < in.txt | grep test | sort > out.txt");
	print_tokens(tokens);
	free_token_list(tokens);
}

void	test_quotes(void)
{
	t_token	*tokens;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   QUOTE HANDLING TESTS                 ║\n");
	printf("╚════════════════════════════════════════╝\n");
	printf("\n✓ Test 1: Single quotes (preserve spaces)\n");
	printf("Input: echo 'hello world'\n");
	tokens = tokenize("echo 'hello world'");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 2: Double quotes\n");
	printf("Input: echo \"test me\"\n");
	tokens = tokenize("echo \"test me\"");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 3: Empty quotes\n");
	printf("Input: echo '' \"\"\n");
	tokens = tokenize("echo '' \"\"");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 4: Mixed quotes\n");
	printf("Input: echo 'single' \"double\" unquoted\n");
	tokens = tokenize("echo 'single' \"double\" unquoted");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 5: Quotes with special chars\n");
	printf("Input: echo '|' '<' '>'\n");
	tokens = tokenize("echo '|' '<' '>'");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test : hami\"d \n");
	printf("Input: echo hami\"d \n");
	tokens = tokenize("echo ham\"i\"d");
	print_tokens(tokens);
	free_token_list(tokens);
}

void	test_expansion(t_env *env)
{
	char	*result;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   VARIABLE EXPANSION TESTS             ║\n");
	printf("╚════════════════════════════════════════╝\n");
	printf("\n✓ Test 1: Variable expansion (double quotes context)\n");
	printf("Input: Hello ham\"$USER\"\n");
	result = expand_variables("Hello ham\"$USER\" ", env, QUOTE_DOUBLE);
	printf("Output: %s\n", result);
	free(result);
	printf("\n✓ Test 2: No expansion (single quotes context)\n");
	printf("Input: 'Hello $USER'\n");
	result = expand_variables("Hello $USER", env, QUOTE_SINGLE);
	printf("Output: '%s'\n", result);
	free(result);
	printf("\n✓ Test 3: Multiple variables (unquoted)\n");
	printf("Input: $USER at $HOME\n");
	result = expand_variables("$USER at $HOME", env, QUOTE_NONE);
	printf("Output: '%s'\n", result);
	free(result);
	printf("\n✓ Test 4: Variable mixed with text (double quotes)\n");
	printf("Input: \"User: $USER, Home: $HOME\"\n");
	result = expand_variables("User: $USER, Home: $HOME", env, QUOTE_DOUBLE);
	printf("Output: '%s'\n", result);
	free(result);
	printf("\n✓ Test 5: Non-existent variable (double quotes)\n");
	printf("Input: \"$NONEXISTENT\"\n");
	result = expand_variables("$NONEXISTENT", env, QUOTE_DOUBLE);
	printf("Output: '%s'\n", result);
	free(result);
	printf("\n✓ Test 6: Lone $ character (double quotes)\n");
	printf("Input: \"Price: $100\"\n");
	result = expand_variables("Price: $100", env, QUOTE_DOUBLE);
	printf("Output: '%s'\n", result);
	free(result);
	printf("\n✓ Test 7: Empty string (double quotes)\n");
	printf("Input: \"\"\n");
	result = expand_variables("", env, QUOTE_DOUBLE);
	printf("Output: '%s'\n", result);
	free(result);
	printf("\n✓ Test 8: Hami\"d\" \n");
	printf("Input: hamid \n");
	result = expand_variables("Hami\"d\" ", env, QUOTE_DOUBLE);
	printf("Output: '%s'\n", result);
	free(result);
}

void	test_edge_cases(void)
{
	t_token	*tokens;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   EDGE CASE TESTS                      ║\n");
	printf("╚════════════════════════════════════════╝\n");
	printf("\n✓ Test 1: Empty input\n");
	tokens = tokenize("");
	printf("Result: %s\n", tokens ? "NOT NULL (ERROR!)" : "NULL (OK)");
	free_token_list(tokens);
	printf("\n✓ Test 2: Only spaces\n");
	tokens = tokenize("     ");
	printf("Result: %s\n", tokens ? "NOT NULL (ERROR!)" : "NULL (OK)");
	free_token_list(tokens);
	printf("\n✓ Test 3: Multiple spaces between tokens\n");
	printf("Input: 'echo     hello     world'\n");
	tokens = tokenize("echo     hello     world");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 4: Operator at start\n");
	printf("Input: '| cat'\n");
	tokens = tokenize("| cat");
	print_tokens(tokens);
	free_token_list(tokens);
	printf("\n✓ Test 5: Operator at end\n");
	printf("Input: 'cat |'\n");
	tokens = tokenize("cat |");
	print_tokens(tokens);
	free_token_list(tokens);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	shell.env = init_env(envp);
	shell.exit_status = 0;
	printf("\n");
	printf("╔══════════════════════════════════════════════════════════╗\n");
	printf("║                                                          ║\n");
	printf("║    Testing: Tokenizer, Quotes, Expansion, AST, Parser    ║\n");
	printf("║                                                          ║\n");
	printf("╚══════════════════════════════════════════════════════════╝\n");
	test_tokenizer();
	test_quotes();
	test_quote_concatenation();
	test_expansion(shell.env);
	test_edge_cases();
	test_ast_quick();
	test_parser();
	debug_environment(shell.env);
	printf("\n");
	printf("╔══════════════════════════════════════════════════════════╗\n");
	printf("║                    TEST COMPLETE                         ║\n");
	printf("╚══════════════════════════════════════════════════════════╝\n");
	printf("\n✅ If all tests show expected output or POSIX behavior → MILESTONE 2 WORKS!\n");
	printf("\n\n\n\n\n\n"); // simply testing builtins now
	test_echo();
	test_pwd();
	test_cd(&shell.env);
	test_exit();
	test_env(shell.env);
	test_env_hidden(&shell.env);
	test_export(&shell.env);
	test_unset(&shell.env);
	// printf("\n\n\n\n\n\n"); // heredoc testing
	// test_heredoc(shell.env);
	free_env_list(&shell.env);
	printf("\n\n\n\n\n\n"); // path finder testing
	shell.env = init_env(envp);
	test_path_finder(shell.env);
	free_env_list(&shell.env);
	return (0);
}
