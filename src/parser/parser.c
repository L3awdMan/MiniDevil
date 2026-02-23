/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:19:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:25:52 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

t_ast	*parse(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	if (validate_syntax(tokens) < 0)
		return (NULL);
	return (parse_pipeline(&tokens));
}
