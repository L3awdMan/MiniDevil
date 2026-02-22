/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:25:03 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/22 03:35:16 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//==================== FUNCTION: exec_left_pipe_child =================
//
// PURPOSE:
//    Execute left side of pipe in child process
//    Redirects stdout to pipe write end
//
// RETURN:
//    void (never returns - child exits)
//
// PARAMETERS:
//    t_ast *left     - Left child of pipe node
//    int pipe_fd[2]  - Pipe file descriptors [read, write]
//    t_shell *shell  - Shell state
//
// VARIABLES:
//    int status - Exit status from recursive execution
//
// ALGORITHM:
//    1. Close read end (left child only writes)
//       // I don't read, so close it
//    2. Redirect stdout to write end: dup2(pipe_fd[1], STDOUT)
//       // make fd 1 point to pipe write end
//    3. Close write end (now duplicated to stdout)
//       // clean up and NOW: fd 1 points to pipe write end
//       // When echo writes to fd 1, data goes IN the pipe
//    4. Recursively execute left child
//       // means run `echo` for example
//    5. Exit with returned status
//
// PSEUDO-CODE:
//    close(pipe_fd[0])              // Don't need read end
//    dup2(pipe_fd[1], STDOUT)       // stdout → pipe write
//    close(pipe_fd[1])              // Close original write end
//    status = executor(left)     // RECURSIVE CALL
//    exit(status)
//
// EXAMPLE:
//    Pipe: echo hello | cat
//    Left child: echo hello
//    → stdout goes to pipe instead of terminal
//    → cat (right child) reads from pipe
/**
 * @note Cleanup inherited (fork) memory before exit to avoid
 *       "still reachable" leaks reported by valgrind
 */

void	exec_left_pipe_child(t_ast *left, int pipe_fd[2], t_shell *shell)
{
	int	status;

	reset_child_signals();
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	status = executor(left, shell);
	free(shell->current_input);
	free_ast(shell->current_ast);
	free_env_list(&shell->env);
	get_next_line(-42);
	exit(status);
}

//==================== FUNCTION: exec_right_pipe_child ================
//
// PURPOSE:
//    Execute right side of pipe in child process
//    Redirects stdin to pipe read end
//
// RETURN:
//    void (never returns - child exits)
//
// PARAMETERS:
//    t_ast *right    - Right child of pipe node
//    int pipe_fd[2]  - Pipe file descriptors
//    t_shell *shell  - Shell state
//
// VARIABLES:
//    int status - Exit status from recursive execution
//
// ALGORITHM:
//    1. Close write end (right child only reads)
//    2. Redirect stdin to read end: dup2(pipe_fd[0], STDIN)
//    3. Close read end (now duplicated to stdin)
//    4. Recursively execute right child
//    5. Exit with returned status
//
// PSEUDO-CODE:
//    close(pipe_fd[1])              // Don't need write end
//    dup2(pipe_fd[0], STDIN)        // stdin ← pipe read
//    close(pipe_fd[0])              // Close original read end
//    status = execute_ast(right)    // RECURSIVE CALL
//    exit(status)
//
// EXAMPLE:
//    Pipe: echo hello | cat
//    Right child: cat
//    → stdin comes from pipe (echo's output)
//    → cat reads from pipe and prints
/**
 * @note Cleanup inherited (fork) memory before exit to avoid
 *       "still reachable" leaks reported by valgrind
 */

void	exec_right_pipe_child(t_ast *right, int pipe_fd[2], t_shell *shell)
{
	int	status;

	reset_child_signals();
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	status = executor(right, shell);
	free(shell->current_input);
	free_ast(shell->current_ast);
	free_env_list(&shell->env);
	get_next_line(-42);
	exit(status);
}

//==================== FUNCTION: wait_for_pipe_children ==================
//
// PURPOSE:
//    Wait for both pipe children and return right child's status
//
// RETURN:
//    int - Exit status of right child (the final command in pipe)
//
// PARAMETERS:
//    pid_t left_pid  - PID of left child
//    pid_t right_pid - PID of right child
//
// VARIABLES:
//    int status      - Status from waitpid
//    int right_status - Final status to return
//
// ALGORITHM:
//    1. Wait for left child
//    2. Wait for right child and save its status
//    3. Extract exit code using WEXITSTATUS
//    4. Return right child's status (pipeline returns last command status)
//
// PSEUDO-CODE:
//    waitpid(left_pid, &status, 0)
//    waitpid(right_pid, &status, 0)
//    right_status = WEXITSTATUS(status)
//    return right_status
//
// EXAMPLE:
//    Pipe: false | true
//    → false exits with 1
//    → true exits with 0
//    → Pipeline returns 0 (right child's status)

static int	wait_for_pipe_children(pid_t left_pid, pid_t right_pid)
{
	int	status;
	int	right_status;

	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		right_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		right_status = 128 + WTERMSIG(status);
	else
		right_status = 1;
	return (right_status);
}

//==================== FUNCTION: handle_pipe =============================
//
// PURPOSE:
//    Execute a pipe node by forking two children and connecting them
//    Example: echo hello | cat
//            left child  |  right child
//
// RETURN:
//    int - Exit status from pipeline (right child's status)
//
// PARAMETERS:
//    t_ast *node    - PIPE node with left and right children
//    t_shell *shell - Shell state
//
// VARIABLES:
//    int pipe_fd[2] - Pipe file descriptors [read_end, write_end]
//    pid_t left_pid - PID of left child (echo)
//    pid_t right_pid - PID of right child (cat)
//
// ALGORITHM:
//    1. Create pipe:
//       - pipe(pipe_fd) creates tunnel
//       - pipe_fd[0] = read end (where data comes OUT)
//       - pipe_fd[1] = write end (where data goes IN)
//
//    2. Fork LEFT child (command before |):
//       - fork() creates copy of process
//       - Child gets pid = 0
//       - Child calls execute_left_pipe_child():
//         * Closes read end (doesn't need it)
//         * Redirects stdout TO pipe write end (output goes IN pipe)
//         * Executes left command
//         * Exits
//       - Parent gets child's pid
//       - Parent continues to step 3
//
//    3. Fork RIGHT child (command after |):
//       - fork() creates another copy of process
//       - Child gets pid = 0
//       - Child calls execute_right_pipe_child():
//         * Closes write end (doesn't need it)
//         * Redirects stdin FROM pipe read end (input comes OUT of pipe)
//         * Executes right command
//         * Exits
//       - Parent gets child's pid
//       - Parent continues to step 4
//
//    4. Parent closes pipe:
//       - Parent closes pipe_fd[0] (read end)
//       - Parent closes pipe_fd[1] (write end)
//       - WHY? Parent doesn't use pipe, only children do
//       - If parent doesn't close, pipe stays open forever
//
//    5. Parent waits for children:
//       - waitpid(left_pid) - wait for left child to finish
//       - waitpid(right_pid) - wait for right child to finish
//       - Extract exit status from right child
//
//    6. Return status:
//       - Return right child's exit status
//       - (Pipeline returns last command's status)
//
// PSEUDO-CODE (SUPER DETAILED):
//
//    // STEP 1: Create the tunnel
//    if (pipe(pipe_fd) fails):
//        print error
//        return 1
//    // Now we have:
//    //   pipe_fd[0] = read end (let's say fd 3)
//    //   pipe_fd[1] = write end (let's say fd 4)
//
//    // STEP 2: Create left child (echo)
//    left_pid = fork()
//    if (left_pid == -1):                    // Fork failed
//        print error
//        return 1
//
//    if (left_pid == 0):                     // I am the LEFT CHILD
//        // Child does this:
//        close(pipe_fd[0])                   // Don't need read end
//        dup2(pipe_fd[1], STDOUT_FILENO)     // fd 1 → pipe write end
//        close(pipe_fd[1])                   // Clean up
//        status = execute_ast(left_node)     // Run echo
//        exit(status)                        // Child dies
//
//    // Parent reaches here (left_pid > 0)
//    // Parent now has left_pid (like 1234)
//
//    // STEP 3: Create right child (cat)
//    right_pid = fork()
//    if (right_pid == -1):                   // Fork failed
//        print error
//        return 1
//
//    if (right_pid == 0):                    // I am the RIGHT CHILD
//        // Child does this:
//        close(pipe_fd[1])                   // Don't need write end
//        dup2(pipe_fd[0], STDIN_FILENO)      // fd 0 → pipe read end
//        close(pipe_fd[0])                   // Clean up
//        status = execute_ast(right_node)    // Run cat
//        exit(status)                        // Child dies
//
//    // Parent reaches here (right_pid > 0)
//    // Parent now has both left_pid and right_pid
//
//    // STEP 4: Parent closes pipe
//    close(pipe_fd[0])    // Parent doesn't read from pipe
//    close(pipe_fd[1])    // Parent doesn't write to pipe
//
//    // STEP 5: Parent waits for children
//    waitpid(left_pid, &status, 0)     // Wait for echo to finish
//    waitpid(right_pid, &status, 0)    // Wait for cat to finish
//
//    // STEP 6: Extract and return status
//    if (WIFEXITED(status)):                // Child exited normally
//        exit_code = WEXITSTATUS(status)    // Get exit code (0-255)
//    else if (WIFSIGNALED(status)):         // Child killed by signal
//        exit_code = 128 + WTERMSIG(status) // Signal convention
//    else:
//        exit_code = 1                      // Unknown error
//
//    return exit_code
//
// VISUAL FLOW:
//
//    PARENT                    LEFT CHILD (echo)           RIGHT CHILD (cat)
//    ------                    -----------------           ------------------
//    pipe() ✓
//    fork() →→→→→→→→→→→→→→→→→ CHILD CREATED
//                              close read end
//                              stdout → pipe write
//                              execute echo
//                              data goes IN pipe →→→→→→→
//                              exit
//
//    fork() →→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→→ CHILD CREATED
//                                                          close write end
//                                                          stdin ← pipe read
//                         →→→→→→→ data comes OUT pipe
//                                                          execute cat
//                                                          print to terminal
//                                                          exit
//
//    close pipes
//    wait for echo ✓
//    wait for cat ✓
//    return cat's status
//
// EXAMPLE EXECUTION: echo hello | cat
//    1. pipe() creates tunnel
//    2. fork() creates echo child:
//       - echo's stdout goes to pipe write end
//       - echo writes "hello\n" → goes into pipe buffer
//       - echo exits
//    3. fork() creates cat child:
//       - cat's stdin comes from pipe read end
//       - cat reads "hello\n" from pipe buffer
//       - cat writes to its stdout (terminal)
//       - cat exits
//    4. Parent closes pipe, waits, returns cat's status (0)

int	handle_pipe(t_ast *node, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipe_fd) == -1)
		return (perror("minishell: pipe"), 1);
	left_pid = fork();
	if (left_pid == -1)
		return (pipe_fork_error(pipe_fd, 0));
	if (left_pid == 0)
		exec_left_pipe_child(node->data.binary.left, pipe_fd, shell);
	right_pid = fork();
	if (right_pid == -1)
		return (pipe_fork_error(pipe_fd, left_pid));
	if (right_pid == 0)
		exec_right_pipe_child(node->data.binary.right, pipe_fd, shell);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (wait_for_pipe_children(left_pid, right_pid));
}
