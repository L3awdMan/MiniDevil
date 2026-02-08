/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/09 00:03:51 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expand tilde at the start of an unquoted word
 *
 * Rules:
 * - `~` alone → $HOME
 * - `~/path` → $HOME/path
 * - Only expands when unquoted (QUOTE_NONE)
 * - Returns original string if no expansion needed
 * @param str The string to expand
 * @param env_list Environment list to get HOME value
 * @param quote_type Quote context of the token
 * @return Expanded string (allocated) or copy of original
 */
char	*expand_tilde(char *str, t_env *env_list, t_quote_type quote_type)
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

/**
 * @brief Chain variable expansion followed by tilde expansion
 *
 * @param str The string to expand
 * @param env Environment list
 * @param qt Quote type of the token
 * @param exit_status Last command exit status
 * @return Fully expanded string (allocated) or NULL on error
 */
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
