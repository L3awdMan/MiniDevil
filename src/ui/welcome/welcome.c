/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:34 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:10:54 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

/**
 * @brief Clear the screen and hide the cursor
 */
static void	init_screen(void)
{
	ft_printf("%s%s", CLEAR_SCREEN, HIDE_CURSOR);
}

/**
 * @brief Wait a bit, show the cursor and clear the screen to exit welcome
 * 
 * @param t Terminal info (used to position the cursor before showing it)
 */
static void	cleanup_screen(t_welcome_term *t)
{
	ft_msleep(800);
	print_at_pos(t->height, 1, SHOW_CURSOR);
	ft_printf("%s", CLEAR_SCREEN);
}

/**
 * @brief Animate top and bottom border sweep across the screen
 * 
 * @param t Terminal dimensions and centre position
 * @warning Since usleep() isn't allowed and ft_msleep() can't handle values 
 * below 100ms, the animation isn't working here
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

/**
 * @brief Draw complete welcome animation sequence and clean up
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
