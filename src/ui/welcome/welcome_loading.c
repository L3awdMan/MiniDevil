/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_loading.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:50:06 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 03:59:50 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

/**
 * @brief Print the credits line centered on the screen
 * 
 * @param t Terminal dimensions
 */
void	animate_credits(t_welcome_term *t)
{
	int			row;
	const char	*credits;

	credits = "✧ Crafted by Lhorse & Pedro ✧";
	row = t->center_y + 9;
	print_centered(t, row, DIM C_AURA4, 0);
	print_at_pos(row, (t->width - 29) / 2, "");
	ft_printf("%s%s%s%s", DIM, C_M3, credits, RESET);
}

/**
 * @brief Animate a progress bar
 * 
 * @param t Terminal dimensions
 */
void	animate_loading(t_welcome_term *t)
{
	int	row;
	int	i;

	row = t->center_y + 11;
	print_centered(t, row, C_AURA2 "░░░░░░░░░░" RESET, 10);
	i = -1;
	while (++i < 10)
	{
		print_at_pos(row, (t->width - 10) / 2 + i, C_M3 "▓" RESET);
		ft_msleep(100);
	}
	ft_msleep(200);
}
