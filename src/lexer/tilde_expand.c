/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:44:03 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "env.h"
#include "libft.h"

/**
 * @brief Expand leading ~ to $HOME
 * 
 * Handles `~` ($HOME) and `~/path` ($HOME/path)
 * 
 * @param str Input string
 * @param env_list Environment list (for HOME lookup)
 * @param quote_type Quote context
 * @return Expanded string
 */
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

/**
 * @brief Handles variable expansion and tilde expansion
 * 
 * Tilde expansions run first to conform with BASH and correctly handling:
 * - `export TEST="~"`
 * - `echo $TEST`
 * 
 * @param str Input string
 * @param env Environment list
 * @param qt Quote type
 * @param exit_status Exit status for $?
 * @return Fully expanded string or NULL on failure
 */
char	*expand_full(char *str, t_env *env, t_quote_type qt, int exit_status)
{
	char	*tilde_exp;
	char	*result;

	tilde_exp = expand_tilde(str, env, qt);
	if (!tilde_exp)
		return (NULL);
	result = expand_variables(tilde_exp, env, qt, exit_status);
	free(tilde_exp);
	return (result);
}
