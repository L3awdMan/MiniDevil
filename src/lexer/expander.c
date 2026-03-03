/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:47:17 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/03 07:10:34 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle $? expansion (last exit status)
 * 
 * @param str Input at `$`
 * @param result Pointer to the result string being built
 * @param exit_status The last exit status
 * @return 2 if $? was handled (len of "$?") and 0 if not a special variable
 */
static int	handle_special_var(char *str, char **result, int exit_status)
{
	char	*value;

	if (str[1] == '?')
	{
		value = ft_itoa(exit_status);
		*result = append_str(*result, value);
		free(value);
		return (2);
	}
	return (0);
}

/**
 * @brief Process a normal $VARIABLE expansion
 * 
 * Extracts the variable name after $ and looks it up in the env list, then
 * appends its value to result. A single `$` appends a literal `$`
 * 
 * @param str Input string
 * @param i Position of the `$` character in str
 * @param result Pointer to the result string being built
 * @param env_list Environment list
 * @return Number of chars consumed (1 for only `$` var_name_len + 1 for `$VAR`)
 */
static int	process_dollar(char *str, int i, char **result, t_env *env_list)
{
	char	*var_name;
	char	*var_value;
	int		len;

	var_name = extract_var_name(str + i + 1, &len);
	if (!var_name)
		return (1);
	if (len == 0)
	{
		*result = append_char(*result, '$');
		free(var_name);
		return (1);
	}
	if (ft_isdigit(var_name[0]) && var_name[0] == '0')
	{
		free(var_name);
		*result = append_str(*result, "minidevil");
		return (2);
	}
	var_value = get_env_value(env_list, var_name);
	if (var_value)
		*result = append_str(*result, var_value);
	free(var_name);
	return (len + 1);
}

/**
 * @brief Dispatch $ expansion (special vars then normal)
 * 
 * @param str Input at the `$` char
 * @param result Pointer to the result string being built
 * @param env_list Environment list
 * @param exit_status Exit status for $?
 * @return Number of characters consumed from str
 */
static int	handle_dollar_sign(char *str, char **result, t_env *env_list,
		int exit_status)
{
	int	skip;

	skip = handle_special_var(str, result, exit_status);
	if (skip > 0)
		return (skip);
	if (is_var_char(str[1]))
		return (process_dollar(str, 0, result, env_list));
	*result = append_char(*result, '$');
	return (1);
}

/**
 * @brief Expand all $VARIABLE in a string
 * 
 * @param str Input string
 * @param env_list Environment list for the variables lookups
 * @param quote_type Quote context (QUOTE_SINGLE = no expansion)
 * @param exit_status Last exit status for $? expansion
 * @return Newly allocated expanded string or NULL
 */
char	*expand_variables(char *str, t_env *env_list, t_quote_type quote_type,
		int exit_status)
{
	char	*result;
	int		i;

	if (quote_type == QUOTE_SINGLE)
		return (ft_strdup(str));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			i += handle_dollar_sign(str + i, &result, env_list, exit_status);
		else
		{
			result = append_char(result, str[i]);
			i++;
		}
		if (!result)
			return (NULL);
	}
	return (result);
}

/**
 * @brief Expand all token values before parsing
 * 
 * - Walks the token list and expands each TOKEN_WORD
 * - Single quoted tokens and heredoc delimiters (token after `<<`) are not exp
 * - A bare `$` connected to a quoted token is replaced with an empty string
 * 
 * @param tokens Token list head
 * @param shell Shell state
 * @return 0 on success and -1 on allocation failure
 * @warning On -1 the token list could be partially expanded
 */
int	expand_all_tokens(t_token *tokens, t_shell *shell)
{
	char	*expanded;
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			if (tokens->quote_type == QUOTE_SINGLE || (prev
					&& prev->type == TOKEN_HEREDOC))
				expanded = ft_strdup(tokens->value);
			else if (is_dollar_quote(tokens))
				expanded = ft_strdup("");
			else
				expanded = expand_full(tokens->value, shell->env,
						tokens->quote_type, shell->exit_status);
			if (!expanded)
				return (-1);
			free(tokens->value);
			tokens->value = expanded;
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}
