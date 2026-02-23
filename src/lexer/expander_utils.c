/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 17:35:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:24:37 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include "token.h"

int	is_dollar_quote(t_token *tok)
{
	if (!tok->connected || !tok->next)
		return (0);
	if (ft_strncmp(tok->value, "$", 2) != 0)
		return (0);
	return (tok->next->quote_type != QUOTE_NONE);
}

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*extract_var_name(char *str, int *len)
{
	int	i;

	i = 0;
	if (str[0] && ft_isdigit(str[0]))
	{
		*len = 1;
		return (ft_substr(str, 0, 1));
	}
	while (str[i] && is_var_char(str[i]))
		i++;
	*len = i;
	return (ft_substr(str, 0, i));
}

char	*append_char(char *str, char c)
{
	char	*new_str;
	int		len;
	int		i;

	len = ft_strlen(str);
	new_str = ft_calloc(sizeof(char), len + 2);
	if (!new_str)
		return (free(str), NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

char	*append_str(char *s1, char *s2)
{
	char	*result;

	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}
