/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:48:16 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/17 17:48:16 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/*
** Get terminal size using ioctl
*/
void	get_term_size(t_term *term)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
	{
		term->width = ws.ws_col;
		term->height = ws.ws_row;
	}
	else
	{
		term->width = 80;
		term->height = 24;
	}
}

/*
** Enable raw mode for char-by-char input
*/
void	set_raw_mode(t_term *term)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &term->orig);
	raw = term->orig;
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/*
** Restore original terminal mode
*/
void	restore_term_mode(t_term *term)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term->orig);
}

/* Prints the goodbye message when exiting the UI */
void	print_goodbye(void)
{
	ft_printf("\n" PURPLE BOLD);
	ft_printf("╔════════════════════════════════════════╗\n");
	ft_printf("║                                        ║\n");
	ft_printf("║           See you next time !          ║\n");
	ft_printf("║                                        ║\n");
	ft_printf("╚════════════════════════════════════════╝\n" RESET "\n");
}
