/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:11:54 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/19 12:00:00 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/* External global from main project's signals.c */
extern volatile sig_atomic_t	g_signal;

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
	setup_ui_signals(shell);
}

static void	check_resize(t_shell *shell)
{
	if (g_signal == SIGWINCH)
	{
		g_signal = 0;
		get_term_size(&shell->ui->term);
		draw_ui(shell->ui);
	}
}

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

void	run_ui_mode(t_shell *shell)
{
	if (!shell)
		return ;
	shell->ui_mode = 1;
	init_ui(shell);
	ui_loop(shell);
	cleanup_term(shell->ui);
	free(shell->ui);
	shell->ui = NULL;
	print_goodbye();
}
