/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_loading.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:50:06 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:35:46 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

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

void	animate_loading(t_welcome_term *t)
{
	int	row;
	int	i;

	row = t->center_y + 11;
	print_centered(t, row, C_AURA2 "░░░░░░░░░░░░░░░░░░░░" RESET, 20);
	i = -1;
	while (++i < 20)
	{
		print_at_pos(row, (t->width - 20) / 2 + i, C_M3 "▓" RESET);
		ft_msleep(50);
	}
	ft_msleep(200);
}
