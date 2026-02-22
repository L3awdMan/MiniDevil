/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:47:17 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/22 07:18:00 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//==================== FUNCTION: handle_special_var ======================
//
// PURPOSE:
//    Handle special shell variables: $? (exit status) and $$ (pid)
//    These variables require special handling separate from env vars
//
// RETURN:
//    int - Number of characters processed (2 for special vars, 0 otherwise)
//
// PARAMETERS:
//    char *str       - Input string starting at $ character
//    char **result   - Pointer to result string being built
//    int exit_status - Last command exit status (for $? expansion)
//
// VARIABLES:
//    char *value - Temporary string for converted number
//
// ALGORITHM:
//    1. Check if character after $ is '?':
//       - Convert exit_status to string with ft_itoa
//       - Append converted value to result
//       - Free temporary string
//       - Return 2 (processed $ and ?)
//    2. Check if character after $ is '$':
//       - Get process ID with getpid()
//       - Convert to string with ft_itoa
//       - Append converted value to result
//       - Free temporary string
//       - Return 2 (processed $ and $)
//    3. If neither special variable:
//       - Return 0 (not handled, caller should process as normal var)
//
// EXAMPLE:
//    Input: "$?", exit_status=42
//    Output: appends "42" to result, returns 2
//
//    Input: "$$"
//    Output: appends "12345" (pid) to result, returns 2
//
//    Input: "$USER"
//    Output: returns 0 (not special, handle as normal variable

static int	handle_special_var(char *str, char **result, int exit_status)
{
	char	*value;

	if (str[1] == '?')
	{
		value = ft_itoa(exit_status);
		*result = append_str(*result, value);
		return (free(value), 2);
	}
	return (0);
}

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

//==================== FUNCTION: handle_dollar_sign ======================
//
// PURPOSE:
//    Process $ character expansion (special vars or normal vars)
//
// RETURN:
//    int - Number of characters to skip
//
// PARAMETERS:
//    char *str       - Input string positioned at $
//    char **result   - Pointer to result string
//    t_env *env_list - Environment list
//    int exit_status - Last exit status
//
// VARIABLES:
//    int skip - Characters processed by special var handler
//
// ALGORITHM:
//    1. Try special variable handling first ($?, $$)
//    2. If handled: return skip count
//    3. If next char is valid var char: process normal variable
//    4. Else: return 1 (skip just the $)

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

//===================== FUNCTION: expand_variables =======================
//
// PURPOSE:
//    Main expansion - replaces all $VARIABLES with their values
//
// RETURN:
//    char * - New string with all expansions performed
//
// PARAMETERS:
//    char *str           - Input string to expand
//    t_env *env_list     - Environment variables linked list
//    t_quote_type quote_type - Quote context
//    int exit_status     - Last command exit status for $?
//
// VARIABLES:
//    char *result - Result string being built
//    int i        - Current position in input string
//
// ALGORITHM:
//    1. If single quotes: return copy without expansion
//    2. Initialize empty result string
//    3. Loop through input:
//       - If $: call handle_dollar_sign()
//       - Else: append character
//    4. Return result

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
	}
	return (result);
}

/**
 * @brief Expand all token values BEFORE parsing
 *
 * Each token is expanded according to its quote_type, with special
 * handling for heredoc delimiters per POSIX specification.
 *
 * @param tokens Token list from tokenizer
 * @param shell Shell state containing env and exit_status
 * @return 0 on success, -1 on memory allocation error
 *
 * @note Expansion rules:
 *       - QUOTE_SINGLE: no expansion (literal string)
 *       - Heredoc delimiter (after TOKEN_HEREDOC): NEVER expanded
 *         regardless of quoting (`<< $VAR`, `<< "$VAR"`, `<< '$VAR'`
 *         all use literal delimiter)
 *       - Otherwise: expand variables ($VAR, $?, $$)
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
