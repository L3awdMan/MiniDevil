/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_for_tests_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:31:50 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/11 23:46:06 by zotaj-di         ###   ########.fr       */
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

void	test_grammar_validation(void)
{
	t_token	*tokens;
	int		result;

	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   GRAMMAR VALIDATION TESTS             ║\n");
	printf("╚════════════════════════════════════════╝\n");
	// ============ TEST 1: Empty Input ============
	printf("\n✓ Test 1: Empty input (NULL)\n");
	tokens = NULL;
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error: empty input\n");
	// ============ TEST 2: Pipe at Start ============
	printf("\n✓ Test 2: Pipe at start\n");
	printf("Input: '| cat'\n");
	tokens = tokenize("| cat");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error near unexpected token `|'\n");
	free_token_list(tokens);
	// ============ TEST 3: Pipe at End ============
	printf("\n✓ Test 3: Pipe at end\n");
	printf("Input: 'cat |'\n");
	tokens = tokenize("cat |");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error near unexpected token `|'\n");
	free_token_list(tokens);
	// ============ TEST 4: Double Pipe ============
	printf("\n✓ Test 4: Double pipe\n");
	printf("Input: 'cat || grep'\n");
	tokens = tokenize("cat || grep");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error near unexpected token `|'\n");
	free_token_list(tokens);
	// ============ TEST 5: Pipe Followed by Redirection ============
	printf("\n✓ Test 5: Pipe followed by redirection\n");
	printf("Input: 'cat | > out.txt'\n");
	tokens = tokenize("cat | > out.txt");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error: missing command after pipe\n");
	free_token_list(tokens);
	// ============ TEST 6: Redirection Without Filename ============
	printf("\n✓ Test 6: Redirection without filename (< at end)\n");
	printf("Input: 'cat <'\n");
	tokens = tokenize("cat <");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error: missing filename after redirection\n");
	free_token_list(tokens);
	// ============ TEST 7: Redirection Followed by Operator ============
	printf("\n✓ Test 7: Redirection followed by operator\n");
	printf("Input: 'cat < |'\n");
	tokens = tokenize("cat < |");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error: missing filename after redirection\n");
	free_token_list(tokens);
	// ============ TEST 8: Multiple Errors (First Error Wins) ============
	printf("\n✓ Test 8: Multiple errors (pipe at start + no filename)\n");
	printf("Input: '| cat <'\n");
	tokens = tokenize("| cat <");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error near unexpected token `|' (first error)\n");
	free_token_list(tokens);
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   VALID SYNTAX TESTS (Should Pass)    ║\n");
	printf("╚════════════════════════════════════════╝\n");
	// ============ VALID TEST 1: Simple Command ============
	printf("\n✓ Valid Test 1: Simple command\n");
	printf("Input: 'echo hello'\n");
	tokens = tokenize("echo hello");
	result = validate_syntax(tokens);
	printf("Result: %d (expected 0 = valid)\n", result);
	free_token_list(tokens);
	// ============ VALID TEST 2: Pipe ============
	printf("\n✓ Valid Test 2: Valid pipe\n");
	printf("Input: 'cat | grep test'\n");
	tokens = tokenize("cat | grep test");
	result = validate_syntax(tokens);
	printf("Result: %d (expected 0 = valid)\n", result);
	free_token_list(tokens);
	// ============ VALID TEST 3: Input Redirection ============
	printf("\n✓ Valid Test 3: Input redirection\n");
	printf("Input: 'cat < input.txt'\n");
	tokens = tokenize("cat < input.txt");
	result = validate_syntax(tokens);
	printf("Result: %d (expected 0 = valid)\n", result);
	free_token_list(tokens);
	// ============ VALID TEST 4: Output Redirection ============
	printf("\n✓ Valid Test 4: Output redirection\n");
	printf("Input: 'echo hello > output.txt'\n");
	tokens = tokenize("echo hello > output.txt");
	result = validate_syntax(tokens);
	printf("Result: %d (expected 0 = valid)\n", result);
	free_token_list(tokens);
	// ============ VALID TEST 5: Multiple Redirections ============
	printf("\n✓ Valid Test 5: Multiple redirections\n");
	printf("Input: 'cat < in.txt > out.txt'\n");
	tokens = tokenize("cat < in.txt > out.txt");
	result = validate_syntax(tokens);
	printf("Result: %d (expected 0 = valid)\n", result);
	free_token_list(tokens);
	// ============ VALID TEST 6: Append Redirection ============
	printf("\n✓ Valid Test 6: Append redirection\n");
	printf("Input: 'echo test >> file.txt'\n");
	tokens = tokenize("echo test >> file.txt");
	result = validate_syntax(tokens);
	printf("Result: %d (expected 0 = valid)\n", result);
	free_token_list(tokens);
	// ============ VALID TEST 7: Heredoc ============
	printf("\n✓ Valid Test 7: Heredoc\n");
	printf("Input: 'cat << EOF'\n");
	tokens = tokenize("cat << EOF");
	result = validate_syntax(tokens);
	printf("Result: %d (expected 0 = valid)\n", result);
	free_token_list(tokens);
	// ============ VALID TEST 8: Complex Pipeline ============
	printf("\n✓ Valid Test 8: Complex pipeline with redirections\n");
	printf("Input: 'cat < in.txt | grep test | sort > out.txt'\n");
	tokens = tokenize("cat < in.txt | grep test | sort > out.txt");
	result = validate_syntax(tokens);
	printf("Result: %d (expected 0 = valid)\n", result);
	free_token_list(tokens);
	printf("\n╔════════════════════════════════════════╗\n");
	printf("║   EDGE CASE TESTS                      ║\n");
	printf("╚════════════════════════════════════════╝\n");
	// ============ EDGE 1: Three Pipes ============
	printf("\n✓ Edge Test 1: Three pipes in a row\n");
	printf("Input: 'cat ||| grep'\n");
	tokens = tokenize("cat ||| grep");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error near unexpected token `|'\n");
	free_token_list(tokens);
	// ============ EDGE 2: Only Redirection ============
	printf("\n✓ Edge Test 2: Only redirection operator\n");
	printf("Input: '>'\n");
	tokens = tokenize(">");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error: missing filename after redirection\n");
	free_token_list(tokens);
	// ============ EDGE 3: Redirection to Redirection ============
	printf("\n✓ Edge Test 3: Redirection followed by redirection\n");
	printf("Input: 'cat < > out.txt'\n");
	tokens = tokenize("cat < > out.txt");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error: missing filename after redirection\n");
	free_token_list(tokens);
	// ============ EDGE 4: Multiple Consecutive Pipes ============
	printf("\n✓ Edge Test 4: Pipe after pipe after pipe\n");
	printf("Input: 'cat | | | grep'\n");
	tokens = tokenize("cat | | | grep");
	result = validate_syntax(tokens);
	printf("Result: %d (expected -1)\n", result);
	printf("Expected: syntax error near unexpected token `|'\n");
	free_token_list(tokens);
	printf("\n╔══════════════════════════════════════════════════════════╗\n");
	printf("║          GRAMMAR VALIDATION TEST COMPLETE                ║\n");
	printf("╚══════════════════════════════════════════════════════════╝\n\n");
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
	printf(" - Total environment variables: %d\n\n", count);
	// Check specific variables
	printf(" - Checking critical variables:\n");
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
	printf("\n - First 10 environment variables:\n");
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
	test_expansion(shell.env);
	test_edge_cases();
	test_ast_quick();
	test_parser();
	debug_environment(shell.env);
	test_grammar_validation();
	printf("\n");
	printf("╔══════════════════════════════════════════════════════════╗\n");
	printf("║                    TEST COMPLETE                         ║\n");
	printf("╚══════════════════════════════════════════════════════════╝\n");
	printf("\n✅ If all tests show expected output or POSIX behavior → MILESTONE 2 WORKS!\n");
	//	printf("\n\n\n\n\n\n"); // simply testing builtins now
	//	test_echo();
	//	test_pwd();
	//	test_cd(&shell.env);
	free_env_list(&shell.env);
	return (0);
}
