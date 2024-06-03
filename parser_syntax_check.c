/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_check.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 15:04:54 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/08 10:16:01 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret_val;

void	syntax_error(int i)
{
	if (i == 1)
		ft_putendl_fd("msh: syntax error near unexpected token `|'", 2);
	if (i == 2)
		ft_putendl_fd("msh: syntax error near unexpected token `newline'", 2);
	if (i == 3)
		ft_putendl_fd("msh: syntax error near unexpected token `<'", 2);
	if (i == 4)
		ft_putendl_fd("msh: syntax error near unexpected token `>'", 2);
	if (i == 5)
		ft_putendl_fd("msh: syntax error near unexpected token `<<'", 2);
	if (i == 6)
		ft_putendl_fd("msh: syntax error near unexpected token `>>'", 2);
	g_ret_val = 2;
}

int	check_syntax_2(char **lex, int i)
{
	if ((lex[i - 1][0] == '<' || lex[i - 1][0] == '>'
		|| lex[i - 1][0] == '|') && lex[i][0] == '|')
		return (syntax_error(1), -1);
	if ((lex[i - 1][0] == '<' || lex[i - 1][0] == '>') && lex[i][0] == '<')
	{
		if (lex[i][1] == '<')
			return (syntax_error(5), -1);
		return (syntax_error(3), -1);
	}
	if ((lex[i - 1][0] == '<' || lex[i - 1][0] == '>') && lex[i][0] == '>')
	{
		if (lex[i][1] == '>')
			return (syntax_error(6), -1);
		return (syntax_error(4), -1);
	}
	return (0);
}

int	check_syntax(char **lex, int i)
{
	if (lex[0][0] == '|')
		return (syntax_error(1), -1);
	if (lex[0][0] == '|' || ((lex[0][0] == '<' ||
		lex[0][0] == '>') && lex[1] == NULL))
		return (syntax_error(2), -1);
	while (lex[i] != NULL)
	{
		if (check_syntax_2(lex, i) != 0)
			return (-1);
		i++;
		if ((lex[i - 1][0] == '<' || lex[i - 1][0] == '>'
			|| lex[i - 1][0] == '|')
			&& lex[i] == NULL)
			return (syntax_error(2), -1);
	}
	return (0);
}
