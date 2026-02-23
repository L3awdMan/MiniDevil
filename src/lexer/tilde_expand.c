/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 23:06:14 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_tilde(char *str, t_env *env_list, t_quote_type quote_type)
{
	char	*home;
	char	*result;

	if (quote_type != QUOTE_NONE)
		return (ft_strdup(str));
	if (!str || str[0] != '~')
		return (ft_strdup(str));
	if (str[1] && str[1] != '/')
		return (ft_strdup(str));
	home = get_env_value(env_list, "HOME");
	if (!home)
		return (ft_strdup(str));
	if (!str[1])
		return (ft_strdup(home));
	result = ft_strjoin(home, str + 1);
	return (result);
}

char	*expand_full(char *str, t_env *env, t_quote_type qt, int exit_status)
{
	char	*var_exp;
	char	*result;

	var_exp = expand_variables(str, env, qt, exit_status);
	if (!var_exp)
		return (NULL);
	result = expand_tilde(var_exp, env, qt);
	free(var_exp);
	return (result);
}
