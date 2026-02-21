/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:50:10 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/21 02:48:51 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

/*
** Sleeps for specified milliseconds.
*/
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
	tmp.c_cc[VTIME] = ms/ 100;
	tcsetattr(STDIN_FILENO, TCSANOW, &tmp);
	read(STDIN_FILENO, &buf, 1);
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

/*
** Gets terminal size and calculates center coordinates.
*/
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

/*
** Prints string at specified position with flush.
*/
void	print_at_pos(int row, int col, const char *str)
{
	ft_printf("\033[%d;%dH%s", row, col, str);
}

/*
** Returns visible length excluding escape sequences.
*/
int	ft_strlen_visible(const char *str)
{
	int	len;
	int	in_escape;

	len = 0;
	in_escape = 0;
	while (*str)
	{
		if (*str == '\033')
			in_escape = 1;
		else if (in_escape && *str == 'm')
			in_escape = 0;
		else if (!in_escape)
			len++;
		str++;
	}
	return (len);
}

/*
** Prints string centered horizontally on given row.
*/
void	print_centered(t_welcome_term *t, int row, const char *str, int len)
{
	int	col;

	col = (t->width - len) / 2;
	if (col < 1)
		col = 1;
	print_at_pos(row, col, str);
}
