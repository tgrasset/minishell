/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 20:08:40 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/08 14:28:39 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret_val;

int	remainder_length(char *str, int end)
{
	int	j;

	j = 0;
	if (str[end] == '}')
		end++;
	while (str[end] != '\0')
	{
		end++;
		j++;
	}
	return (j);
}

int	get_var_name_len(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '{' && ft_strchr(str, '}') != NULL)
	{
		while (str[i] != '}')
			i++;
		return (i);
	}
	if (str[i] == '?')
		return (1);
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n'
		&& str[i] != '$' && str[i] != 34 && str[i] != 39 && str[i] != ']'
		&& str[i] != '=' && str[i] != '/')
		i++;
	return (i);
}

void	copy_brackets_var(char *str, char *var_name, int *end, int start)
{
	(*end)++;
	while (str[*end] != '}')
	{
		var_name[*end - start - 2] = str[*end];
		(*end)++;
	}
	var_name[*end - start - 2] = '\0';
}

void	copy_nobrackets_var(char *str, char *var_name, int *end, int start)
{
	if (str[*end] == '?')
	{
		var_name[0] = '?';
		var_name[1] = '\0';
		return ;
	}
	while (str[*end] && str[*end] != ' ' && str[*end] != '\t'
		&& str[*end] != '\n' && str[*end] != '$' && str[*end] != 34
		&& str[*end] != 39 && str[*end] != '=' && str[*end] != '/'
		&& str[*end] != ']')
	{
		var_name[*end - start - 1] = str[*end];
		(*end)++;
	}
	var_name[*end - start - 1] = '\0';
}

char	*replace_by_ret_value(t_sh *sh, char *str, int start, int end)
{
	char	*exp;

	exp = ft_itoa(g_ret_val);
	if (exp == NULL)
		ft_error(sh, 1);
	if (str[start + 1] == '?')
		end++;
	str = replace_2(str, exp, start, end);
	free(exp);
	if (str == NULL)
		ft_error(sh, 1);
	return (str);
}
