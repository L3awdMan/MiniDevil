/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:05 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 03:31:20 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Return the length in bytes of an UTF-8 character
 * 
 * @param c First byte of the UTF-8 sequence
 * @return 4, 3, 2 or 1 for ASCII
 */
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

/**
 * @brief Copy up to max_width visible characters into a static buffer
 * 
 * Handles UTF-8 sequences
 * 
 * @param line Source string to truncate (could be NULL)
 * @param max_width Maximum number to include
 * @return Pointer to static buffer holding the truncated result
 * @note The static buffer gets overwritten on every call
 */
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

/**
 * @brief Calculate the visual display width of a UTF-8 string in terminal cols
 * 
 * 4 byte (emoji range) are counted as 2 columns, the others as 1
 * 
 * @param s UTF-8 string (NULL terminated)
 * @return Visual column width of the string
 */
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
