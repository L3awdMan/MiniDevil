/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:36:49 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/11/30 17:33:55 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_ptr_len(uintptr_t ptr)
{
	int	length;

	length = 0;
	while (ptr != 0)
	{
		length++;
		ptr /= 16;
	}
	return (length);
}

void	ft_put_ptr(uintptr_t ptr)
{
	if (ptr >= 16)
	{
		ft_put_ptr((ptr / 16));
		ft_put_ptr((ptr % 16));
	}
	else
	{
		if (ptr <= 9)
			ft_print_char((ptr + '0'));
		else
			ft_print_char(ptr - 10 + 'a');
	}
}

int	ft_print_ptr(uintptr_t ptr)
{
	int	length;

	length = 0;
	if (ptr == 0)
		length += write(1, "(nil)", 5);
	else
	{
		length += write(1, "0x", 2);
		ft_put_ptr(ptr);
		length += ft_ptr_len(ptr);
	}
	return (length);
}
