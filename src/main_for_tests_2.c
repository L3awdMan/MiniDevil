/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_for_tests_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:31:50 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/18 19:42:20 by zotaj-di         ###   ########.fr       */
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

#define YELLOW "\x1b[33m"
#define GRAY "\x1b[90m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

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

void	test_variable_expansion(t_env *env)
{
	char	*result;
	int		exit_status;

	printf("\n╔═══════════════════════════════════════════╗\n");
	printf("║   VARIABLE EXPANSION TESTS ($? and $$)   ║\n");
	printf("╚═══════════════════════════════════════════╝\n");
	// Set a test exit status
	exit_status = 42;
	// ============ TEST 1: Basic $VAR ============
	printf("\n✓ Test 1: Basic $VAR expansion\n");
	printf("Input: \"Hello $USER\"\n");
	result = expand_variables("Hello $USER", env, QUOTE_NONE, exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: 'Hello %s'\n", get_env_value(env, "USER"));
	free(result);
	// ============ TEST 2: $? (exit status) ============
	printf("\n✓ Test 2: $? expansion (exit status)\n");
	printf("Input: \"Last exit: $?\"\n");
	printf("Exit status: %d\n", exit_status);
	result = expand_variables("Last exit: $?", env, QUOTE_NONE, exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: 'Last exit: 42'\n");
	free(result);
	// ============ TEST 3: $$ (process id) ============
	printf("\n✓ Test 3: $$ expansion (process id)\n");
	printf("Input: \"PID: $$\"\n");
	printf("Actual PID: %d\n", getpid());
	result = expand_variables("PID: $$", env, QUOTE_NONE, exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: 'PID: %d'\n", getpid());
	free(result);
	// ============ TEST 4: Multiple expansions ============
	printf("\n✓ Test 4: Multiple expansions in one string\n");
	printf("Input: \"$USER got $? errors (pid:$$)\"\n");
	result = expand_variables("$USER got $? errors (pid:$$)", env, QUOTE_NONE,
			exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: '%s got 42 errors (pid:%d)'\n", get_env_value(env,
			"USER"), getpid());
	free(result);
	// ============ TEST 5: Single quotes (no expansion) ============
	printf("\n✓ Test 5: Single quotes - no expansion\n");
	printf("Input: '$USER $? $$' (single quotes)\n");
	result = expand_variables("$USER $? $$", env, QUOTE_SINGLE, exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: '$USER $? $$' (unchanged)\n");
	free(result);
	// ============ TEST 6: Double quotes (expansion) ============
	printf("\n✓ Test 6: Double quotes - with expansion\n");
	printf("Input: \"User: $USER, Status: $?\" (double quotes)\n");
	result = expand_variables("User: $USER, Status: $?", env, QUOTE_DOUBLE,
			exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: 'User: %s, Status: 42'\n", get_env_value(env, "USER"));
	free(result);
	// ============ TEST 7: Undefined variable ============
	printf("\n✓ Test 7: Undefined variable (expands to empty)\n");
	printf("Input: \"Value: $UNDEFINED_VAR_XYZ\"\n");
	result = expand_variables("Value: $UNDEFINED_VAR_XYZ", env, QUOTE_NONE,
			exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: 'Value: ' (empty expansion)\n");
	free(result);
	// ============ TEST 8: $? with different exit codes ============
	printf("\n✓ Test 8: $? with exit code 0\n");
	exit_status = 0;
	printf("Input: \"Status: $?\"\n");
	printf("Exit status: %d\n", exit_status);
	result = expand_variables("Status: $?", env, QUOTE_NONE, exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: 'Status: 0'\n");
	free(result);
	printf("\n✓ Test 9: $? with exit code 127\n");
	exit_status = 127;
	printf("Input: \"Error $?\"\n");
	printf("Exit status: %d\n", exit_status);
	result = expand_variables("Error $?", env, QUOTE_NONE, exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: 'Error 127'\n");
	free(result);
	// ============ TEST 10: Edge case - just $ ============
	printf("\n✓ Test 10: Lone $ character\n");
	printf("Input: \"Price: $100\"\n");
	result = expand_variables("Price: $100", env, QUOTE_NONE, exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: 'Price: 100' ($ treated as literal)\n");
	free(result);
	// ============ TEST 11: $$ multiple times ============
	printf("\n✓ Test 11: $$ appearing multiple times\n");
	printf("Input: \"$$ and $$ are same\"\n");
	result = expand_variables("$$ and $$ are same", env, QUOTE_NONE,
			exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: '%d and %d are same'\n", getpid(), getpid());
	free(result);
	// ============ TEST 12: Mixed $VAR, $?, $$ ============
	printf("\n✓ Test 12: Complex mix\n");
	printf("Input: \"[$?] $USER($$) at $HOME\"\n");
	exit_status = 1;
	result = expand_variables("[$?] $USER($$) at $HOME", env, QUOTE_NONE,
			exit_status);
	printf("Output: '%s'\n", result);
	printf("Expected: '[1] %s(%d) at %s'\n", get_env_value(env, "USER"),
		getpid(), get_env_value(env, "HOME"));
	free(result);
	printf("\n╔═══════════════════════════════════════════╗\n");
	printf("║   EXPANSION TESTS COMPLETE               ║\n");
	printf("╚═══════════════════════════════════════════╝\n");
}

// ============ DEBUG ===========
void	test_count_words(void)
{
	t_token	*tokens;
	t_token	*curr;
	int		i;

	tokens = tokenize("echo test > /tmp/debug.txt");
	printf("Testing count_word_tokens():\n");
	printf("Should count 2 (echo, test)\n");
	printf("Actually counts: %d\n", count_word_tokens(tokens));
	// Check token types
	curr = tokens;
	i = 0;
	while (curr)
	{
		printf("  Token[%d]: type=%d (%s)\n", i, curr->type,
			curr->type == 0 ? "WORD" : "OPERATOR");
		curr = curr->next;
		i++;
	}
	free_token_list(tokens);
}

void	test_tokenizer_only(void)
{
	t_token	*tokens;
	t_token	*curr;
	int		i;

	printf("\n╔═══════════════════════════════════════╗\n");
	printf("║   TOKENIZER DEBUG TEST                ║\n");
	printf("╚═══════════════════════════════════════╝\n");
	printf("\nInput: 'echo test > /tmp/debug.txt'\n\n");
	tokens = tokenize("echo test > /tmp/debug.txt");
	printf("Tokens created:\n");
	curr = tokens;
	i = 0;
	while (curr)
	{
		printf("  [%d] type=%d value='%s'\n", i, curr->type, curr->value);
		printf("       Token type names:\n");
		printf("         0=TOKEN_WORD\n");
		printf("         1=TOKEN_PIPE\n");
		printf("         2=TOKEN_REDIR_IN\n");
		printf("         3=TOKEN_REDIR_OUT\n");
		printf("         4=TOKEN_APPEND\n");
		printf("         5=TOKEN_HEREDOC\n");
		if (curr->type == 3)
			printf(" ✅ This is TOKEN_REDIR_OUT!\n");
		else if (curr->type == 0 && curr->value[0] == '>')
			printf(" ❌ ERROR: '>' is TOKEN_WORD, should be TOKEN_REDIR_OUT!\n");
		curr = curr->next;
		i++;
	}
	printf("\n");
	printf("Expected tokens:\n");
	printf("  [0] type=0 (WORD) value='echo'\n");
	printf("  [1] type=0 (WORD) value='test'\n");
	printf("  [2] type=3 (REDIR_OUT) value='>'\n");
	printf("  [3] type=0 (WORD) value='/tmp/debug.txt'\n");
	free_token_list(tokens);
}

void	test_redirection_debug(t_shell *shell)
{
	t_token	*tokens;
	t_ast	*tree;
	int		status;
	int		fd;
	char	buf[100] = {0};
	ssize_t	n;

	printf("\n╔═══════════════════════════════════════╗\n");
	printf("║   REDIRECTION DEBUG TEST              ║\n");
	printf("╚═══════════════════════════════════════╝\n");
	// Test: echo test > /tmp/debug.txt
	printf("\n1. Parsing: echo test > /tmp/debug.txt\n");
	tokens = tokenize("echo test > /tmp/debug.txt");
	tree = parse(tokens);
	// Print AST structure
	printf("2. AST Root node type: %d\n", tree->type);
	printf("   Expected: %d (NODE_REDIR_OUT)\n", NODE_REDIR_OUT);
	if (tree->type == NODE_REDIR_OUT)
	{
		printf("   ✅ Redirection node created correctly\n");
		printf("   File: '%s'\n", tree->data.redir.file);
		printf("   Command node type: %d (should be %d)\n",
			tree->data.redir.cmd->type, NODE_COMMAND);
	}
	else
	{
		printf("   ❌ ERROR: Root is not REDIR_OUT!\n");
		printf("   This means parser didn't create redirection node.\n");
	}
	// Execute
	printf("\n3. Executing...\n");
	printf("   (If you see 'test' below, redirection failed)\n");
	status = executor(tree, shell);
	printf("\n4. Exit status: %d\n", status);
	// Check file
	printf("\n5. Checking if file was created...\n");
	fd = open("/tmp/debug.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("   ❌ FILE NOT CREATED\n");
		perror("   open");
	}
	else
	{
		printf("   ✅ File exists!\n");
		n = read(fd, buf, 99);
		if (n > 0)
		{
			buf[n] = '\0';
			printf("   File contents: '%s'\n", buf);
			if (strcmp(buf, "test\n") == 0)
				printf("   ✅ Correct content!\n");
			else
				printf("   ❌ Wrong content (expected 'test\\n')\n");
		}
		close(fd);
	}
	// Cleanup
	free_ast(tree);
	free_token_list(tokens);
	system("rm -f /tmp/debug.txt");
}

void	test_executor(t_shell *shell)
{
	t_token	*tokens;
	t_ast	*tree;
	int		status;

	printf("\n╔═══════════════════════════════════════╗\n");
	printf("║   EXECUTOR TEST (Issue #15)           ║\n");
	printf("╚═══════════════════════════════════════╝\n");
	// Test 1: Simple command
	printf("\n✓ Test 1: Simple command\n");
	printf("Input: echo hello\n");
	tokens = tokenize("echo hello");
	tree = parse(tokens);
	status = executor(tree, shell);
	printf("Exit status: %d (expected 0)\n", status);
	free_ast(tree);
	free_token_list(tokens);
	// Test 2: Output redirection
	printf("\n✓ Test 2: Output redirection\n");
	printf("Input: echo test > /tmp/minishell_test.txt\n");
	tokens = tokenize("echo test > /tmp/minishell_test.txt");
	tree = parse(tokens);
	status = executor(tree, shell);
	printf("Exit status: %d (expected 0)\n", status);
	printf("Check file: cat /tmp/minishell_test.txt\n");
	system("cat /tmp/minishell_test.txt");
	free_ast(tree);
	free_token_list(tokens);
	// Test 3: Input redirection
	printf("\n✓ Test 3: Input redirection\n");
	printf("Input: echo < /tmp/minishell_test.txt\n");
	tokens = tokenize("echo < /tmp/minishell_test.txt");
	tree = parse(tokens);
	status = executor(tree, shell);
	printf("Exit status: %d (expected 0)\n", status);
	free_ast(tree);
	free_token_list(tokens);
	// Test 4: Simple pipe
	printf("\n✓ Test 4: Simple pipe\n");
	printf("Input: echo hello | cat\n");
	tokens = tokenize("echo hello | cat");
	tree = parse(tokens);
	status = executor(tree, shell);
	printf("Exit status: %d (expected 0)\n", status);
	free_ast(tree);
	free_token_list(tokens);
	// Test 5: Complex: pipe + redirection
	printf("\n✓ Test 5: Pipe + redirection\n");
	printf("Input: ls < /tmp/minishell_test.txt | grep test\n");
	tokens = tokenize("ls < /tmp/minishell_test.txt | grep test");
	tree = parse(tokens);
	status = executor(tree, shell);
	printf("Exit status: %d (expected 0)\n", status);
	free_ast(tree);
	free_token_list(tokens);
	// Cleanup
	system("rm -f /tmp/minishell_test.txt");
	printf("\n╔═══════════════════════════════════════╗\n");
	printf("║   EXECUTOR TEST COMPLETE              ║\n");
	printf("╚═══════════════════════════════════════╝\n");
}

// ═══════════════════════════════════════════════════════════════════════
// ISSUE #11: SIMPLE COMMAND PARSING TESTS
// ═══════════════════════════════════════════════════════════════════════
/*
void	test_issue_11_simple_commands(void)
{
	t_token	*tokens;
	t_ast	*tree;

	printf("\n╔═══════════════════════════════════════════════════════════╗\n");
	printf("║   ISSUE #11: SIMPLE COMMAND PARSING                      ║\n");
	printf("╚═══════════════════════════════════════════════════════════╝\n");
	// TEST 1: Basic command with no args
	printf(YELLOW "\n✓ Test 1: echo\n" RESET);
	tokens = tokenize("echo");
	tree = parse(tokens);
	if (tree && tree->type == NODE_COMMAND && tree->data.cmd.argc == 1)
		printf(GREEN "✅ PASS: argc=1, arg[0]='%s'\n" RESET,
			tree->data.cmd.args[0]);
	else
		printf(RED "❌ FAIL: Expected NODE_COMMAND with argc=1\n" RESET);
	free_ast(tree);
	free_token_list(tokens);
	// TEST 2: Command with arguments
	printf(YELLOW "\n✓ Test 2: echo hello world\n" RESET);
	tokens = tokenize("echo hello world");
	tree = parse(tokens);
	if (tree && tree->type == NODE_COMMAND && tree->data.cmd.argc == 3)
		printf(GREEN "✅ PASS: argc=3, args=['%s','%s','%s']\n" RESET,
			tree->data.cmd.args[0], tree->data.cmd.args[1],
			tree->data.cmd.args[2]);
	else
		printf(RED "❌ FAIL: Expected argc=3, got argc=%d\n" RESET,
			tree ? tree->data.cmd.argc : -1);
	free_ast(tree);
	free_token_list(tokens);
	// TEST 3: Command with quoted argument
	printf(YELLOW "\n✓ Test 3: echo 'hello world'\n" RESET);
	tokens = tokenize("echo 'hello world'");
	tree = parse(tokens);
	if (tree && tree->type == NODE_COMMAND && tree->data.cmd.argc == 2)
		printf(GREEN "✅ PASS: argc=2, arg[1]='%s'\n" RESET,
			tree->data.cmd.args[1]);
	else
		printf(RED "❌ FAIL\n" RESET);
	free_ast(tree);
	free_token_list(tokens);
	// TEST 4: Path command
	printf(YELLOW "\n✓ Test 4: /bin/ls -la\n" RESET);
	tokens = tokenize("/bin/ls -la");
	tree = parse(tokens);
	if (tree && tree->type == NODE_COMMAND && tree->data.cmd.argc == 2)
		printf(GREEN "✅ PASS: Full path command works\n" RESET);
	else
		printf(RED "❌ FAIL\n" RESET);
	free_ast(tree);
	free_token_list(tokens);
}
*/
// ═══════════════════════════════════════════════════════════════════════
// ISSUE #12: REDIRECTION PARSING TESTS (CRITICAL!)
// ═══════════════════════════════════════════════════════════════════════

void	test_issue_12_redirections(void)
{
	t_token	*tokens;
	t_ast	*tree;
	t_ast	*cmd;

	printf("\n╔═══════════════════════════════════════════════════════════╗\n");
	printf("║                   REDIRECTION PARSING                     ║\n");
	printf("╚═══════════════════════════════════════════════════════════╝\n");
	// TEST 1: Output redirection
	printf(YELLOW "\n✓ Test 1: echo hello > out.txt\n" RESET);
	tokens = tokenize("echo hello > out.txt");
	tree = parse(tokens);
	if (tree && tree->type == NODE_REDIR_OUT)
	{
		printf(GREEN "✅ PASS: Root is REDIR_OUT\n" RESET);
		printf("   File: '%s'\n", tree->data.redir.file);
		if (tree->data.redir.cmd && tree->data.redir.cmd->type == NODE_COMMAND)
			printf("   Command argc: %d\n",
				tree->data.redir.cmd->data.cmd.argc);
		else
			printf(RED "   ❌ No command node!\n" RESET);
	}
	else
		printf(RED "❌ FAIL: Expected NODE_REDIR_OUT, got type=%d\n" RESET,
			tree ? tree->type : -1);
	free_ast(tree);
	free_token_list(tokens);
	// TEST 2: Input redirection
	printf(YELLOW "\n✓ Test 2: cat < input.txt\n" RESET);
	tokens = tokenize("cat < input.txt");
	tree = parse(tokens);
	if (tree && tree->type == NODE_REDIR_IN)
	{
		printf(GREEN "✅ PASS: Root is REDIR_IN, file='%s'\n" RESET,
			tree->data.redir.file);
	}
	else
		printf(RED "❌ FAIL\n" RESET);
	free_ast(tree);
	free_token_list(tokens);
	// TEST 3: Multiple redirections (TRICKY!)
	printf(YELLOW "\n✓ Test 3: cat < in.txt > out.txt\n" RESET);
	tokens = tokenize("cat < in.txt > out.txt");
	tree = parse(tokens);
	if (tree)
	{
		printf("   Root type: %d (should be REDIR_OUT=%d or REDIR_IN=%d)\n",
			tree->type, NODE_REDIR_OUT, NODE_REDIR_IN);
		if (tree->type == NODE_REDIR_OUT)
		{
			printf("   Out file: '%s'\n", tree->data.redir.file);
			if (tree->data.redir.cmd
				&& tree->data.redir.cmd->type == NODE_REDIR_IN)
				printf(GREEN "✅ PASS: Nested redirections!\n" RESET);
			else
				printf(RED "❌ FAIL: Missing nested REDIR_IN\n" RESET);
		}
	}
	else
		printf(RED "❌ FAIL: NULL tree\n" RESET);
	free_ast(tree);
	free_token_list(tokens);
	// TEST 4: Append redirection
	printf(YELLOW "\n✓ Test 4: echo test >> file.txt\n" RESET);
	tokens = tokenize("echo test >> file.txt");
	tree = parse(tokens);
	if (tree && tree->type == NODE_REDIR_APPEND)
		printf(GREEN "✅ PASS: APPEND works\n" RESET);
	else
		printf(RED "❌ FAIL: Expected APPEND\n" RESET);
	free_ast(tree);
	free_token_list(tokens);
	// TEST 5: Heredoc
	printf(YELLOW "\n✓ Test 5: cat << EOF\n" RESET);
	tokens = tokenize("cat << EOF");
	tree = parse(tokens);
	if (tree && tree->type == NODE_REDIR_HEREDOC)
		printf(GREEN "✅ PASS: HEREDOC parsed, delimiter='%s'\n" RESET,
			tree->data.redir.file);
	else
		printf(RED "❌ FAIL\n" RESET);
	free_ast(tree);
	free_token_list(tokens);
	// TEST 6: EDGE CASE - Redirection BEFORE command
	printf(YELLOW "\n✓ Test 6: < in.txt cat\n" RESET);
	tokens = tokenize("< in.txt cat");
	tree = parse(tokens);
	if (tree)
	{
		printf("   Type: %d\n", tree->type);
		if (tree->type == NODE_REDIR_IN)
			printf(GREEN "✅ PASS: Handles redir before command\n" RESET);
		else
			printf(RED "❌ FAIL: Should be REDIR_IN\n" RESET);
	}
	free_ast(tree);
	free_token_list(tokens);
	// TEST 7: EDGE CASE - Mixed args and redirections
	printf(YELLOW "\n✓ Test 7: echo hello > out.txt world\n" RESET);
	printf(RED "⚠️  THIS IS THE TRICKY ONE Check if 'world' is in command args!\n" RESET);
	tokens = tokenize("echo hello > out.txt hamid");
	tree = parse(tokens);
	if (tree)
	{
		printf("   Root type: %d\n", tree->type);
		// This should parse as:
		// REDIR_OUT(file=out.txt, cmd=COMMAND(echo, hello, world))
		// OR it might fail if parser doesn't handle args after redirections
		if (tree->type == NODE_REDIR_OUT && tree->data.redir.cmd)
		{
			cmd = tree->data.redir.cmd;
			printf("   Command argc: %d\n", cmd->data.cmd.argc);
			if (cmd->data.cmd.argc == 3)
				printf(GREEN "   ✅ PASS: 'world' is in args!\n" RESET);
			else
				printf(RED "   ❌ FAIL: argc=%d, missing 'world'!\n" RESET,
					cmd->data.cmd.argc);
		}
		if (tree->type == NODE_REDIR_OUT && tree->data.redir.cmd)
		{
			cmd = tree->data.redir.cmd;
			printf("   Command argc: %d\n", cmd->data.cmd.argc);
			// ADD THIS:
			printf("   Args are: ");
			for (int i = 0; i < cmd->data.cmd.argc; i++)
				printf("'%s' ", cmd->data.cmd.args[i]);
			printf("\n");
		}
	}
	free_ast(tree);
	free_token_list(tokens);
}

// ═══════════════════════════════════════════════════════════════════════
// PIPELINE PARSING TESTS
// ═══════════════════════════════════════════════════════════════════════

void	test_pipelines(void)
{
	t_token	*tokens;
	t_ast	*tree;

	printf("\n╔═══════════════════════════════════════════════════════════╗\n");
	printf("║                     PIPELINE PARSING                      ║\n");
	printf("╚═══════════════════════════════════════════════════════════╝\n");
	// TEST 1: Simple pipe
	printf(YELLOW "\n✓ Test 1: ls | grep test\n" RESET);
	tokens = tokenize("ls | grep test");
	tree = parse(tokens);
	if (tree && tree->type == NODE_PIPE)
	{
		printf(GREEN "✅ PASS: Root is PIPE\n" RESET);
		printf("   Left type: %d (should be %d=COMMAND)\n",
			tree->data.binary.left->type, NODE_COMMAND);
		printf("   Right type: %d (should be %d=COMMAND)\n",
			tree->data.binary.right->type, NODE_COMMAND);
	}
	else
		printf(RED "❌ FAIL\n" RESET);
	free_ast(tree);
	free_token_list(tokens);
	// TEST 2: Triple pipe
	printf(YELLOW "\n✓ Test 2: cat | grep word | wc -l\n" RESET);
	tokens = tokenize("cat | grep word | wc -l");
	tree = parse(tokens);
	if (tree && tree->type == NODE_PIPE)
	{
		printf(GREEN "✅ PASS: Root is PIPE\n" RESET);
		// Tree should be: PIPE(PIPE(cat, grep), wc)
		if (tree->data.binary.left && tree->data.binary.left->type == NODE_PIPE)
			printf(GREEN "   ✅ Left is also PIPE (correct structure!)\n" RESET);
		else
			printf(RED "   ❌ Left should be PIPE for 3-command pipeline\n" RESET);
	}
	free_ast(tree);
	free_token_list(tokens);
	// TEST 3: Pipe with redirections
	printf(YELLOW "\n✓ Test 3: cat < in.txt | grep test > out.txt\n" RESET);
	tokens = tokenize("cat < in.txt | grep test > out.txt");
	tree = parse(tokens);
	if (tree && tree->type == NODE_PIPE)
		printf(GREEN "✅ PASS: Complex pipeline parsed\n" RESET);
	else
		printf(RED "❌ FAIL\n" RESET);
	free_ast(tree);
	free_token_list(tokens);
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
	// test_tokenizer();
	//	test_quotes();
	//	test_expansion(shell.env);
	//	test_edge_cases();
	//	test_ast_quick();
	//	test_parser();
	//	debug_environment(shell.env);
	//	test_grammar_validation();
	//	test_issue_11_simple_commands();
	// test_issue_12_redirections();
	// test_pipelines();
	// printf("\n");
	// printf("╔══════════════════════════════════════════════════════════╗\n");
	// printf("║                    TEST COMPLETE                         ║\n");
	// printf("╚══════════════════════════════════════════════════════════╝\n");
	// printf("\n✅ If all tests show expected output or POSIX behavior → MILESTONE 2 WORKS!\n");
	// 	printf("\n\n\n\n\n\n"); // testing builtins and path finder
	//	test_echo();
	//	test_pwd();
	//	test_cd(&shell.env);
	//	test_exit();
	//	test_env(shell.env);
	//	test_env_hidden(&shell.env);
	//	test_export(&shell.env);
	//	test_unset(&shell.env);
	//	test_path_finder(shell.env);
	// printf("\n\n\n\n\n\n"); // testing exec external
	// test_exec_external(&shell.env);
	//	test_executor(&shell);
	// test_redirection_debug(&shell);
	//	test_tokenizer_only();
	// DEBUG :
	// test_count_words();
	test_variable_expansion(shell.env);
	free_env_list(&shell.env);
	return (0);
}
