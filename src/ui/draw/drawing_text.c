#include "minishell_ui.h"

/*
** Truncates line to max width, returns static buffer.
*/
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

/*
** Returns byte length of UTF-8 character.
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

/*
** Returns visual width of string (handles UTF-8).
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
