/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_extract.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 12:31:26 by tgrasset          #+#    #+#             */
/*   Updated: 2023/02/13 09:47:22 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_token(t_sh *sh, int *i, int *j)
{
	sh->lex[*i] = copy_token("|", 1);
	(*j)++;
}

void	in_redir_token(t_sh *sh, int *i, int *j)
{
	if (sh->buf[*j + 1] == '<')
	{
		sh->lex[*i] = copy_token("<<", 2);
		*j += 2;
	}
	else
	{
		sh->lex[*i] = copy_token("<", 1);
		(*j)++;
	}
}

void	out_redir_token(t_sh *sh, int *i, int *j)
{
	if (sh->buf[*j + 1] == '>')
	{
		sh->lex[*i] = copy_token(">>", 2);
		*j += 2;
	}
	else
	{
		sh->lex[*i] = copy_token(">", 1);
		(*j)++;
	}
}

void	quote_token(t_sh *sh, int *i, int *j, int type)
{
	if (type == 34)
	{
		sh->lex[*i] = copy_token(&sh->buf[*j], 34);
		(*j)++;
		while (sh->buf[*j] && sh->buf[*j] != 34)
			(*j)++;
		(*j)++;
	}
	else
	{
		sh->lex[*i] = copy_token(&sh->buf[*j], 39);
		(*j)++;
		while (sh->buf[*j] && sh->buf[*j] != 39)
			(*j)++;
		(*j)++;
	}
}

void	word_token(t_sh *sh, int *i, int *j)
{
	sh->lex[*i] = copy_token(&sh->buf[*j], 42);
	while (sh->buf[*j] && sh->buf[*j] != '|' && sh->buf[*j] != '<'
		&& sh->buf[*j] != '>' && sh->buf[*j] != ' ' && sh->buf[*j] != '\t'
		&& sh->buf[*j] != '\n')
	{	
		if ((sh->buf[*j] == 34 && ft_strchr(&sh->buf[*j + 1], 34) != NULL)
			|| (sh->buf[*j] == 39 && ft_strchr(&sh->buf[*j + 1], 39) != NULL))
			skip_quotes(sh->buf, j, sh->buf[*j]);
		else
			(*j)++;
	}
}
