/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:05 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/26 03:57:23 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

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

char	*truncate_line(const char *line, int max_width)
{
	static char	buf[BUF_SIZE];
	int			i;
	int			visual_w;
	int			char_len;

	if (!line)
		return ("");
	ft_memset(buf, 0, sizeof(buf));
	i = 0;
	visual_w = 0;
	while (line[i] && visual_w < max_width)
	{
		char_len = utf8_byte_len((unsigned char)line[i]);
		if (i + char_len > BUF_SIZE - 1)
			break ;
		ft_memcpy(buf + i, line + i, char_len);
		i += char_len;
		visual_w++;
	}
	buf[i] = '\0';
	return (buf);
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
