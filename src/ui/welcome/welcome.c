/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:34 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/21 00:39:42 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

/*
** Initializes screen for welcome animation.
*/
static void	init_screen(void)
{
	ft_printf("%s%s", CLEAR_SCREEN, HIDE_CURSOR);
}

/*
** Cleans up screen after welcome animation.
*/
static void	cleanup_screen(t_welcome_term *t)
{
	ft_msleep(800);
	print_at_pos(t->height, 1, SHOW_CURSOR);
	ft_printf("%s", CLEAR_SCREEN);
}

/*
** Draws animated border effect on top and bottom.
*/
static void	draw_border_effect(t_welcome_term *t)
{
	int	i;
	int	row;

	row = t->center_y - 13;
	i = -1;
	while (++i < t->width)
	{
		print_at_pos(row, i + 1, C_AURA1 "▄" RESET);
		if (i % 3 == 0)
			ft_msleep(5);
	}
	row = t->center_y + 12;
	i = t->width;
	while (--i >= 0)
	{
		print_at_pos(row, i + 1, C_AURA1 "▀" RESET);
		if (i % 3 == 0)
			ft_msleep(5);
	}
}

/*
** Main welcome screen - runs full animation sequence.
*/
void	draw_welcome(void)
{
	t_welcome_term	t;

	get_welcome_term_size(&t);
	init_screen();
	draw_border_effect(&t);
	ft_msleep(200);
	animate_title(&t);
	ft_msleep(300);
	animate_quote(&t);
	ft_msleep(400);
	animate_demon(&t);
	flash_demon_eyes(&t);
	ft_msleep(300);
	animate_credits(&t);
	animate_loading(&t);
	cleanup_screen(&t);
}
