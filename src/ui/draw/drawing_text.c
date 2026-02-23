/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:05 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:29:19 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

char	*truncate_line(const char *line, int max_width)
{
	static char	buf[1024];
	int			len;

	if (!line)
		return ("");
	len = ft_strlen(line);
	if (len > max_width)
		len = max_width;
	ft_memset(buf, 0, sizeof(buf));
	ft_memcpy(buf, line, len);
	return (buf);
}

static int	utf8_byte_len(unsigned char c)
{
	if (c >= 0xF0)
		return (4);
	if (c >= 0xE0)
		return (3);
	if (c >= 0xC0)
		return (2);
	return (1);
}

int	visual_strlen(const char *s)
{
	int	len;

	len = 0;
	while (*s)
	{
		if ((unsigned char)*s >= 0xF0)
			len += 2;
		else
			len++;
		s += utf8_byte_len((unsigned char)*s);
	}
	return (len);
}
