/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:19:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 23:30:40 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

//==================== FUNCTION: parse ===================================
//
// PURPOSE:
//    Main parser entry point - convert tokens to AST
//
// RETURN:
//    t_ast * - Root of AST tree, NULL on error
//
// PARAMETERS:
//    t_token *tokens - Token list from lexer
//
// ALGORITHM:
//    1. Validate input (check for NULL)
//    2. Call parse_pipeline to build tree
//    3. Return the resulting AST
//
// EXAMPLE:
//    tokens = [cat, file, |, grep, test]
//    parse(tokens) -> PIPE(CMD(cat), CMD(grep))

t_ast	*parse(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	if (validate_syntax(tokens) < 0)
		return (NULL);
	return (parse_pipeline(&tokens));
}
