/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_count.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 12:27:01 by tgrasset          #+#    #+#             */
/*   Updated: 2023/02/13 16:23:16 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_lex(char **lex)
{
	int	i;

	i = 0;
	if (lex == NULL)
		return ;
	while (lex[i] != NULL)
	{
		free(lex[i]);
		i++;
	}
	free(lex);
}

static void	skip_one_or_two(char *s, int *i)
{
	if (s[*i + 1] == s[*i])
		(*i) += 2;
	else
		(*i)++;
}

void	skip_quotes(char *s, int *i, int type)
{
	if (type == 34)
	{
		(*i)++;
		while (s[*i] && s[*i] != 34)
			(*i)++;
		(*i)++;
	}
	else
	{
		(*i)++;
		while (s[*i] && s[*i] != 39)
			(*i)++;
		(*i)++;
	}
}

static void	skip_word(char *s, int *i)
{
	while (s[*i] && s[*i] != '|' && s[*i] != '<' && s[*i] != '>'
		&& s[*i] != ' ' && s[*i] != '\t' && s[*i] != '\n')
	{
		if ((s[*i] == 34 && ft_strchr(&s[*i + 1], 34) != NULL)
			|| (s[*i] == 39 && ft_strchr(&s[*i + 1], 39) != NULL))
			skip_quotes(s, i, s[*i]);
		else
			(*i)++;
	}
}

int	token_count(char *s)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n'))
			i++;
		if (s[i] == '\0')
			break ;
		if (s[i] == '|')
			i++;
		else if (s[i] == '<' || s[i] == '>')
			skip_one_or_two(s, &i);
		else
			skip_word(s, &i);
		count++;
	}
	return (count);
}
