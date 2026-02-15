#include "minishell_ui.h"

/*
** Adds the welcome message box to the output panel.
*/
static void	add_welcome_msg(t_ui *ui)
{
	out_add_line(&ui->out, "");
	out_add_line(&ui->out,
		"  ╔══════════════════════════════════════════╗");
	out_add_line(&ui->out,
		"  ║  Welcome to Minishell UI!                ║");
	out_add_line(&ui->out,
		"  ║                                          ║");
	out_add_line(&ui->out,
		"  ║  • Type commands and press Enter         ║");
	out_add_line(&ui->out,
		"  ║  • Use ↑/↓ to scroll output              ║");
	out_add_line(&ui->out,
		"  ║  • Press Ctrl-D to exit                  ║");
	out_add_line(&ui->out,
		"  ║  • Type 'clear' to clear output          ║");
	out_add_line(&ui->out,
		"  ║                                          ║");
	out_add_line(&ui->out,
		"  ╚══════════════════════════════════════════╝");
	out_add_line(&ui->out, "");
}

/*
** Prints the goodbye message when exiting the UI.
*/
static void	print_goodbye(void)
{
	ft_printf("\n" PURPLE BOLD);
	ft_printf("╔════════════════════════════════════════╗\n");
	ft_printf("║                                        ║\n");
	ft_printf("║           See you next time !          ║\n");
	ft_printf("║                                        ║\n");
	ft_printf("╚════════════════════════════════════════╝\n" RESET "\n");
}

/*
** Initialize UI structure and components.
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
	setup_ui_signals(shell);
}

/*
** Main UI loop - handles input and drawing.
*/
static void	ui_loop(t_shell *shell)
{
	int	key;

	draw_welcome();
	ft_msleep(2000);
	/* Hide cursor again after welcome animation showed it */
	write(STDOUT_FILENO, CURSOR_HIDE, ft_strlen(CURSOR_HIDE));
	add_welcome_msg(shell->ui);
	draw_ui(shell->ui);
	while (shell->ui->running)
	{
		key = read_key();
		if (key == KEY_ENTER)
		{
			handle_key(shell, key);
			draw_ui(shell->ui);
		}
		else
		{
			handle_key(shell, key);
			if (key != KEY_UP && key != KEY_DOWN)
				redraw_cmd_only(shell->ui);
		}
	}
}

/*
** Main entry point for UI mode.
** Called from main() when --ui flag is provided.
*/
void	run_ui_mode(t_shell *shell)
{
	if (!shell)
		return ;
	
	/* Initialize UI mode */
	shell->ui_mode = 1;
	init_ui(shell);
	
	/* Run UI loop */
	ui_loop(shell);
	
	/* Cleanup - note: out_free() is already called in cleanup_term() */
	cleanup_term(shell->ui);
	free(shell->ui);
	shell->ui = NULL;
	print_goodbye();
}
