#include "minishell.h"

static char	*extract_quoted(char *str, int *len)
{
	char	quote;
	int		end;
	char	*content;

	quote = str[0];
	end = 1;
	while (str[end] && str[end] != quote) // find closing quote
		end++;
	if (str[end] != quote)
	{
		ft_putstr_fd("minishell: unclosed quote\n", 2);
		return (NULL);
	}
	content = ft_substr(str, 0, end + 1);
	*len = end + 1; // go the next character (right after the closing quote)
	return (content);
}

/**
 * @brief extract normal text until a separator (space, operator or quote)
 */
static char *extract_unquoted(char *str, int *len)
{
	int	i;

	i = 0;
	while (str[i] && !is_word_end(str[i]))
		i++;
	*len = i;
	return (ft_substr(str, 0, i));
}

//==================================================
//=============== TOKEN PROCESSING =================
//==================================================

/**
 * @brief Get the next chunk of the word (quoted or unquoted)
 */
char	*get_next_word_chunk(char *input, int *chunk_len)
{
	if (input[0] == '\'' || input[0] == '"')
		return (extract_quoted(input, chunk_len));
	return (extract_unquoted(input, chunk_len));
}
