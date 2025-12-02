/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_nbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:52:21 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/11/30 17:33:39 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_nbr(int nb)
{
	int		length;
	long	nbr;

	length = 0;
	nbr = nb;
	if (nbr < 0)
	{
		length += write(1, "-", 1);
		nbr = -nbr;
	}
	if (nbr > 9)
		length += ft_print_nbr(nbr / 10);
	length += ft_print_char((nbr % 10) + '0');
	return (length);
}
