/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:11:54 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:55:18 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Allocate and initialize the t_ui struct and attach it to shell
 * 
 * Calls init_term and setup_ui_signals after allocation
 * 
 * @param shell Shell state (receives the new UI pointer)
 * @warning On allocation failure it calls exit(1)
 */
static void	init_ui(t_shell *shell)
{
	t_ui	*ui;

	ui = ft_calloc(1, sizeof(t_ui));
	if (!ui)
	{
		ft_putendl_fd("Error: UI allocation failed", STDERR_FILENO);
		exit(1);
	}
	shell->ui = ui;
	ui->running = 1;
	init_term(ui);
	setup_ui_signals();
}

/**
 * @brief Handle a SIGWINCH signal (get new terminal dimensions and redraw)
 * 
 * @param shell Shell state
 */
static void	check_resize(t_shell *shell)
{
	if (g_signal == SIGWINCH)
	{
		g_signal = 0;
		get_term_size(&shell->ui->term);
		draw_ui(shell->ui);
	}
}

/**
 * @brief Run the main loop
 * 
 * - Show the welcome animation, wait ~2s and enter the event loop
 * - Read keys, dispatche handle_key() and redraw
 * - Exit when shell->ui->running is set to 0
 * 
 * @param shell Shell state
 */
static void	ui_loop(t_shell *shell)
{
	int	key;

	draw_welcome();
	ft_msleep(2000);
	write(STDOUT_FILENO, CURSOR_HIDE, ft_strlen(CURSOR_HIDE));
	add_welcome_msg(shell->ui);
	draw_ui(shell->ui);
	while (shell->ui->running)
	{
		check_resize(shell);
		key = read_key();
		handle_key(shell, key);
		if (key == KEY_ENTER)
			draw_ui(shell->ui);
		else if (key != KEY_UP && key != KEY_DOWN)
			redraw_cmd_only(shell->ui);
	}
}

/**
 * @brief Entry point for UI mode
 * 
 * Return silently if shell is NULL, not a tty or terminal is too small
 * 
 * @param shell Shell state
 */
void	run_ui_mode(t_shell *shell)
{
	struct winsize	ws;

	if (!shell)
		return ;
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return ;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
	{
		ft_putstr_fd("Error: failed to get terminal size\n", STDERR_FILENO);
		return ;
	}
	if (ws.ws_col < MIN_WIDTH || ws.ws_row < MIN_HEIGHT)
	{
		ft_putstr_fd("Error: terminal too small for UI mode\n", STDERR_FILENO);
		return ;
	}
	shell->ui_mode = 1;
	init_ui(shell);
	ui_loop(shell);
	cleanup_term(shell->ui);
	free(shell->ui);
	shell->ui = NULL;
	print_goodbye();
}
