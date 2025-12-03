/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 22:44:42 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/02 23:49:42 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

//--------- Environment Variable Node Structure --------------------------
//
// Why double linked list?
// - Easy insertion/deletion (for export/unset)
// - Can traverse forward/backward
// - Each node holds ONE environment variable
//
// Structure:
//    NODE: "PATH=/usr/bin:/bin"
//    key = "PATH"
//    value = "/usr/bin:/bin"
//    full = "PATH=/usr/bin:/bin"
//
// Example list:
//    [HOME=/home/user] <-> [PATH=/bin] <-> [USER=john] <-> NULL
//
// Environment Structure :
//   key   : Variable name (e.g., "PATH")
//   value : Variable value (e.g., "/usr/bin:/bin")
//   full  : Full string "KEY=VALUE" (for display)
//   next  : Next Node
//   prev  : Previous Node

typedef struct s_env
{
	char			*key;
	char			*value;
	char			*full;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

//------- MAIN SHELL STRUCTURE --------------------------
//
// PURPOSE: Store all shell state in one place
//
// WHY?
// - Easier to pass around (one pointer instead of many)
// - Clean way to manage global state
// - Easy to add more fields later
//
//
//   env         : Head of environment list
//   exit_status : Last command exit status ($?)
//   running     : Shell is running flag

typedef struct s_shell
{
	t_env			*env;
	int				exit_status;
	int				running;
}					t_shell;

// Environment initialization
t_env				*init_env(char **envp);
t_env				*create_env_node(char *env_string);
void				add_env_node(t_env **head, t_env *new_node);

#endif 
