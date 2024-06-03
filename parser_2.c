/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 11:32:23 by tgrasset          #+#    #+#             */
/*   Updated: 2023/02/16 15:36:41 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	option_nbr(char **lex, int i)
{
	int	count;

	count = 0;
	if (lex[i] != NULL && lex[i][0] != '|')
	{
		i++;
		count++;
	}
	while (lex[i] != NULL && lex[i][0] != '|')
	{
		if (lex[i][0] != '<' && lex[i][0] != '>' && lex[i - 1][0] != '<'
				&& lex[i - 1][0] != '>')
			count++;
		i++;
	}
	return (count);
}

int	get_command_name(t_sh *sh, t_comm *new, int i)
{
	int	j;

	j = i;
	new->file = NULL;
	while (sh->lex[j] != NULL && sh->lex[j][0] != '|' && ((sh->lex[j][0] == '<'
		|| sh->lex[j][0] == '>') || (j != i && ((sh->lex[j - 1][0] == '<')
		|| sh->lex[j - 1][0] == '>'))))
		j++;
	if (sh->lex[j] == NULL || sh->lex[j][0] == '|')
		new->file = NULL;
	else
	{
		new->file = ft_strdup(sh->lex[j]);
		if (new->file == NULL)
			ft_error(sh, 1);
	}
	return (j);
}

void	get_command_args(t_sh *sh, t_comm *new, int i, int j)
{
	new->argv = NULL;
	new->argv = malloc(sizeof(char *) * (option_nbr(sh->lex, i) + 1));
	if (new->argv == NULL)
		ft_error(sh, 1);
	if (new->file != NULL)
	{
		new->argv[j] = ft_strdup(sh->lex[i]);
		i++;
		j++;
	}
	while (sh->lex[i] != NULL && sh->lex[i][0] != '|')
	{
		if (sh->lex[i][0] != '<' && sh->lex[i][0] != '>'
				&& sh->lex[i - 1][0] != '<' && sh->lex[i - 1][0] != '>')
		{
			new->argv[j] = ft_strdup(sh->lex[i]);
			if (new->argv[j] == NULL)
				ft_error(sh, 1);
			j++;
		}
		i++;
	}
	new->argv[j] = NULL;
}
