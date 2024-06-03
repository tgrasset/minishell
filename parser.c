/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 10:28:12 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/06 17:48:20 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	assign_redir_values(t_sh *sh, t_redir *new, int i)
{
	if (ft_strlen(sh->lex[i]) == 1)
		new->doubl = 0;
	else
		new->doubl = 1;
	if (sh->lex[i][0] == '<')
		new->output = 0;
	else
		new->output = 1;
	if (sh->lex[i + 1] == NULL || sh->lex[i + 1][0] == '<'
		|| sh->lex[i + 1][0] == '>' || sh->lex[i + 1][0] == '|')
		new->name = NULL;
	else
	{
		new->name = ft_strdup(sh->lex[i + 1]);
		if (new->name == NULL)
			ft_error(sh, 1);
	}
	new->fd = -42;
	new->next = NULL;
}

void	add_redir(t_sh *sh, t_comm *cmd, int i)
{
	t_redir	*new;
	t_redir	*tmp;

	tmp = cmd->redir;
	new = malloc(sizeof(t_redir));
	if (new == NULL)
		ft_error(sh, 1);
	assign_redir_values(sh, new, i);
	if (new->output == 0)
		cmd->infile = 1;
	if (new->output == 1)
		cmd->outfile = 1;
	if (cmd->redir == NULL)
		cmd->redir = new;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	get_redirections(t_sh *sh, t_comm *cmd, int i)
{
	int	j;

	j = i;
	while (sh->lex[j] != NULL && sh->lex[j][0] != '|')
	{
		if (sh->lex[j][0] == '<' || sh->lex[j][0] == '>')
			add_redir(sh, cmd, j);
		j++;
	}
}

void	add_command(t_sh *sh, int i)
{
	t_comm	*new;
	t_comm	*tmp;

	tmp = sh->comm;
	new = malloc(sizeof(t_comm));
	if (new == NULL)
		ft_error(sh, 1);
	new->next = NULL;
	if (sh->comm == NULL)
		sh->comm = new;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
	assign_cmd_value(new);
	get_redirections(sh, new, i);
	get_command_args(sh, new, get_command_name(sh, new, i), 0);
}

void	parsing(t_sh *sh)
{
	int	i;

	i = 0;
	sh->pipe_number = 0;
	sh->comm = NULL;
	if (sh->lex[0] != NULL && check_syntax(sh->lex, 1) != 0)
		return ;
	while (sh->lex[i] != NULL)
	{
		add_command(sh, i);
		while (sh->lex[i] != NULL && sh->lex[i][0] != '|')
			i++;
		if (sh->lex[i] != NULL)
		{
			i++;
			sh->pipe_number++;
		}
	}
	clean_paired_quotes(sh);
}
