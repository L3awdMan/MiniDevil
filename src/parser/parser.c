/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:19:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/03 08:00:48 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

/**
 * @brief Parse a token list into an AST
 * 
 * Validates syntax and then delegates to parse_pipeline() which handles the
 * recursive descent
 * 
 * @param tokens Head of the token list
 * @return Root of the AST or NULL on syntax error / empty input
 * @warning The caller has to free the returned AST with free_ast()
 */
t_ast	*parse(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	if (validate_syntax(tokens) < 0)
		return (NULL);
	return (parse_pipeline(&tokens));
}
