/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_box.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:48:56 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/22 07:04:53 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/*
** Draws the top border of a box.
*/
static void	draw_box_top(t_box *b)
{
	print_at(b->y, b->x, b->color);
	write(STDOUT_FILENO, BOX_TL, 3);
	draw_hline(b->w - 2, b->color);
	write(STDOUT_FILENO, BOX_TR, 3);
	ft_printf("%s", RESET);
}

/*
** Draws the bottom border of a box.
*/
void	draw_box_bottom(t_box *b)
{
	print_at(b->y + b->h - 1, b->x, b->color);
	write(STDOUT_FILENO, BOX_BL, 3);
	draw_hline(b->w - 2, b->color);
	write(STDOUT_FILENO, BOX_BR, 3);
	ft_printf("%s", RESET);
}

/*
** Draws the left and right sides of a box.
*/
void	draw_box_sides(t_box *b)
{
	int	i;

	i = 1;
	while (i < b->h - 1)
	{
		print_at(b->y + i, b->x, b->color);
		write(STDOUT_FILENO, BOX_V, 3);
		ft_printf("%s", RESET);
		print_at(b->y + i, b->x + b->w - 1, b->color);
		write(STDOUT_FILENO, BOX_V, 3);
		ft_printf("%s", RESET);
		i++;
	}
}

/*
** Draws a complete box with all borders.
*/
void	draw_box(t_box *b)
{
	draw_box_top(b);
	draw_box_sides(b);
	draw_box_bottom(b);
}

/*
** Draws a box top border with centered title.
*/
void	draw_box_title(t_box *b, const char *title)
{
	int	title_len;
	int	pad;

	title_len = visual_strlen(title) + 2;
	pad = (b->w - title_len - 2) / 2;
	print_at(b->y, b->x, b->color);
	write(STDOUT_FILENO, BOX_TL, 3);
	draw_hline(pad, b->color);
	ft_printf(" %s%s%s ", BOLD, title, RESET);
	ft_printf("%s", b->color);
	draw_hline(b->w - 2 - title_len - pad, b->color);
	write(STDOUT_FILENO, BOX_TR, 3);
	ft_printf("%s", RESET);
}
