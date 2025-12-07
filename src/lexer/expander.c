/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:47:17 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/07 16:52:37 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "token.h"

//===================== FUNCTION: process_dollar =========================
//
// PURPOSE:
//    Handle $ expansion at position i in string
//
// RETURN:
//    int - Number of characters to skip
//
// PARAMETERS:
//    char *str       - Input string
//    int i           - Current position (at $)
//    char **result   - Result string being built
//    t_env *env_list - Environment list
//
// VARIABLES:
//    char *var_name  - Variable name
//    char *var_value - Value from env
//    int len         - Length of var name
//
// ALGORITHM:
//    1. Extract variable name after $
//    2. If empty: append $, free variable name, return 1
//    3. Look up value in environment
//    4. If found: append to result
//    5. Free var_name
//    6. Return len + 1 (skip $ + var_name)

static int	process_dollar(char *str, int i, char **result, t_env *env_list)
{
	char	*var_name;
	char	*var_value;
	int		len;

	var_name = extract_var_name(str + i + 1, &len);
	if (len == 0)
	{
		*result = append_char(*result, '$');
		free(var_name);
		return (1);
	}
	var_value = get_env_value(env_list, var_name);
	if (var_value)
		*result = append_str(*result, var_value);
	free(var_name);
	return (len + 1);
}

//===================== FUNCTION: expand_variables =======================
//
// PURPOSE:
//    Replace $VARIABLE with its value from environment
//
// RETURN:
//    char * - String with variables expanded
//
// PARAMETERS:
//    char *str       - Input string
//    t_env *env_list - Environment list
//    int in_quotes   - 0=no expand, 1=expand
//
// VARIABLES:
//    char *result - Result string
//    int i        - Position in str
//
// ALGORITHM:
//    1. If in_quotes == 0: return copy (no expansion)
//    2. Initialize result = ""
//    3. Loop through string:
//       - If $: process_dollar, skip returned amount
//       - Else: append char, i++
//    4. Return result
//
//========================================================================

char	*expand_variables(char *str, t_env *env_list, int in_quotes)
{
	char	*result;
	int		i;

	if (in_quotes == 0)
		return (ft_strdup(str));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && is_var_char(str[i + 1]))
			i += process_dollar(str, i, &result, env_list);
		else
		{
			result = append_char(result, str[i]);
			i++;
		}
	}
	return (result);
}
