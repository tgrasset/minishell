/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 22:20:53 by tgrasset          #+#    #+#             */
/*   Updated: 2023/02/17 16:32:21 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_remove_2(char *new, char *str, int i, int j)
{
	char	quote;

	while (str[i] != '\0')
	{
		if ((str[i] == 34 && ft_strchr(&str[i + 1], 34) != NULL)
			|| (str[i] == 39 && ft_strchr(&str[i + 1], 39) != NULL))
		{
			quote = str[i++];
			while (str[i] != '\0' && str[i] != quote)
			{
				new[j] = str[i];
				i++;
				j++;
			}
			if (str[i] == quote)
				i++;
		}
		else
		{
			new[j] = str[i];
			i++;
			j++;
		}
	}
	new[j] = '\0';
}

char	*quote_remove(char *str, t_sh *sh)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (str == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		if ((str[i] == 34 && ft_strchr(&str[i + 1], 34) != NULL)
			|| (str[i] == 39 && ft_strchr(&str[i + 1], 39) != NULL))
		{
			j += 2;
			skip_quotes(str, &i, str[i]);
		}
		else
			i++;
	}
	new = malloc(sizeof(char) * (ft_strlen(str) - j + 1));
	if (new == NULL)
		ft_error(sh, 1);
	quote_remove_2(new, str, 0, 0);
	free(str);
	return (new);
}

void	clean_paired_quotes(t_sh *sh)
{
	t_comm	*tmp;
	t_redir	*tmp2;
	int		i;

	tmp = sh->comm;
	while (tmp != NULL)
	{
		i = -1;
		tmp->file = quote_remove(tmp->file, sh);
		while (tmp->argv[++i] != NULL)
			tmp->argv[i] = quote_remove(tmp->argv[i], sh);
		tmp2 = tmp->redir;
		while (tmp2 != NULL)
		{
			tmp2->name = quote_remove(tmp2->name, sh);
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
}

void	assign_cmd_value(t_comm *new)
{
	new->redir = NULL;
	new->argv = NULL;
	new->file = NULL;
	new->in_out_fail = 0;
	new->infile = 0;
	new->outfile = 0;
	new->pid = -42;
	new->stdout_save = -42;
}
