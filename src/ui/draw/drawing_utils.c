/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:09 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 23:02:34 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

void	print_at(int row, int col, const char *str)
{
	ft_printf("\033[%d;%dH%s", row, col, str);
}

void	draw_hline(int width, const char *color)
{
	int	i;

	if (color)
		write(STDOUT_FILENO, color, ft_strlen(color));
	i = 0;
	while (i++ < width)
		write(STDOUT_FILENO, "─", 3);
	if (color)
		write(STDOUT_FILENO, RESET, ft_strlen(RESET));
}
