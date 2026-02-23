/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:50:10 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 23:02:53 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

void	ft_msleep(int ms)
{
	struct termios	old;
	struct termios	tmp;
	char			buf;

	if (ms <= 0)
		return ;
	tcgetattr(STDIN_FILENO, &old);
	tmp = old;
	tmp.c_cc[VMIN] = 0;
	tmp.c_cc[VTIME] = ms / 100;
	tcsetattr(STDIN_FILENO, TCSANOW, &tmp);
	read(STDIN_FILENO, &buf, 1);
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

void	get_welcome_term_size(t_welcome_term *t)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
	{
		t->width = 80;
		t->height = 24;
	}
	else
	{
		t->width = ws.ws_col;
		t->height = ws.ws_row;
	}
	t->center_x = t->width / 2;
	t->center_y = t->height / 2;
}

void	print_at_pos(int row, int col, const char *str)
{
	ft_printf("\033[%d;%dH%s", row, col, str);
}

void	print_centered(t_welcome_term *t, int row, const char *str, int len)
{
	int	col;

	col = (t->width - len) / 2;
	if (col < 1)
		col = 1;
	print_at_pos(row, col, str);
}
