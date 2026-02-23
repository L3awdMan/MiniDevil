/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_effects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:50:02 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:35:58 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

static void	print_mystic_char(char c, int idx)
{
	const char	*mystic[] = {C_M1, C_M2, C_M3, C_M4, C_M5};

	ft_printf("%s%s%c", BOLD, mystic[idx % 5], c);
}

void	animate_title(t_welcome_term *t)
{
	const char	*title = ">>> MINIHELL <<<";
	int			row;
	int			col;
	int			i;

	row = t->center_y - 12;
	col = (t->width - 16) / 2;
	print_at_pos(row - 1, col - 2, C_AURA3 "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" RESET);
	print_at_pos(row + 1, col - 2, C_AURA3 "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" RESET);
	print_at_pos(row, col - 2, C_AURA5 "█" RESET);
	print_at_pos(row, col + 17, C_AURA5 "█" RESET);
	print_at_pos(row, col, "");
	i = -1;
	while (title[++i])
	{
		print_mystic_char(title[i], i);
		ft_msleep(40);
	}
	ft_printf("%s", RESET);
}

static void	print_quote_lines(t_welcome_term *t, int row)
{
	const char	*q1;
	const char	*q2;
	const char	*q3;

	q1 = "\"Unix is simple. It just takes a genius";
	q2 = "to understand its simplicity.\"";
	q3 = "- Dennis Ritchie";
	print_centered(t, row + 1, C_AURA3 "║ " RESET, 2);
	print_at_pos(row + 1, (t->width + 44) / 2 - 1, C_AURA3 " ║" RESET);
	print_at_pos(row + 1, (t->width - 40) / 2 + 1, DIM C_M4);
	ft_printf("%s%s", q1, RESET);
	ft_msleep(100);
	print_centered(t, row + 2, C_AURA3 "║ " RESET, 2);
	print_at_pos(row + 2, (t->width + 44) / 2 - 1, C_AURA3 " ║" RESET);
	print_at_pos(row + 2, (t->width - 32) / 2 + 1, DIM C_M4);
	ft_printf("%s%s", q2, RESET);
	ft_msleep(100);
	print_centered(t, row + 3, C_AURA3 "║ " RESET, 2);
	print_at_pos(row + 3, (t->width + 44) / 2 - 1, C_AURA3 " ║" RESET);
	print_at_pos(row + 3, (t->width - 18) / 2 + 1, BOLD C_M5);
	ft_printf("%s%s", q3, RESET);
}

void	animate_quote(t_welcome_term *t)
{
	int	row;

	row = t->center_y - 9;
	print_centered(t, row,
		C_AURA3 "╔════════════════════════════════════════════╗" RESET, 46);
	ft_msleep(100);
	print_quote_lines(t, row);
	print_centered(t, row + 4,
		C_AURA3 "╚════════════════════════════════════════════╝" RESET, 46);
}
