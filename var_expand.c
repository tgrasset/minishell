/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 16:14:58 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/03 15:28:34 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_exp(char *new, char *exp, int *i)
{
	int	j;

	j = 0;
	while (exp[j] != '\0')
	{
		new[*i] = exp[j];
		j++;
		(*i)++;
	}
}

char	*replace_2(char *str, char *exp, int start, int end)
{
	char	*new;
	int		i;

	i = -1;
	new = malloc(sizeof(char) * (start + ft_strlen(exp)
				+ remainder_length(str, end) + 1));
	if (new == NULL)
		return (NULL);
	while (++i < start)
		new[i] = str[i];
	if (exp != NULL)
		append_exp(new, exp, &i);
	if (str[end] && str[end] == '}')
		end++;
	while (str[end] != '\0')
	{
		new[i] = str[end];
		i++;
		end++;
	}
	new[i] = '\0';
	free(str);
	return (new);
}

void	free_var_expansion(char *exp, char *str, t_sh *sh)
{
	if (exp != NULL)
		free(exp);
	if (str == NULL)
		ft_error(sh, 1);
}

char	*replace_var(char *str, t_sh *sh, int start, int *exp_len)
{
	int		end;
	char	*var_name;
	char	*exp;

	end = start + 1;
	var_name = malloc(sizeof(char) * (get_var_name_len(&str[end]) + 1));
	if (var_name == NULL)
		ft_error(sh, 1);
	if (str[end] == '{' && ft_strchr(str, '}') != NULL)
		copy_brackets_var(str, var_name, &end, start);
	else
		copy_nobrackets_var(str, var_name, &end, start);
	if (var_name[0] == '?' && var_name[1] == '\0')
	{
		free(var_name);
		*exp_len = 1;
		return (replace_by_ret_value(sh, str, start, end));
	}
	exp = ft_getenv(var_name, sh->env);
	free(var_name);
	*exp_len = ft_strlen(exp);
	str = replace_2(str, exp, start, end);
	free_var_expansion(exp, str, sh);
	return (str);
}

void	expand_variables(t_sh *sh, int in_d_quotes, int i, int exp_len)
{
	if (sh->buf == NULL)
		return ;
	while (sh->buf[i] != '\0')
	{
		if (in_d_quotes == 0 && sh->buf[i] == '"'
			&& ft_strchr(&sh->buf[i + 1], '"') != NULL)
			in_d_quotes = 1;
		else if (in_d_quotes == 1 && sh->buf[i] == '"')
			in_d_quotes = 0;
		if (in_d_quotes == 0 && sh->buf[i] == 39
			&& ft_strchr(&sh->buf[i + 1], 39) != NULL)
			skip_quotes(sh->buf, &i, 39);
		if (sh->buf[i] && sh->buf[i] == '$' && sh->buf[i + 1] != '$'
			&& sh->buf[i + 1] != ' ' && sh->buf[i + 1] != '\t'
			&& sh->buf[i + 1] != '\n' && sh->buf[i + 1] != '\0'
			&& sh->buf[i + 1] != '"' && sh->buf[i + 1] != '='
			&& sh->buf[i + 1] != '/'
			&& in_hd_delimiter(sh->buf, i, in_d_quotes) == 0)
		{
			sh->buf = replace_var(sh->buf, sh, i, &exp_len);
			i = i + exp_len - 1;
		}
		if (sh->buf[i] != '\0')
			i++;
	}
}
