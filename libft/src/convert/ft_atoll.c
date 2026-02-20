/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 06:46:57 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/20 07:01:18 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_overflow(long long result, int digit, int sign)
{
	if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
		return (1);
	if (sign == -1 && (-result < (LLONG_MIN + digit) / 10))
		return (1);
	return (0);
}

long long	ft_atoll(const char *str, int *overflow)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	*overflow = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		if (check_overflow(result, *str - '0', sign))
		{
			*overflow = 1;
			return (0);
		}
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
