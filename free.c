/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 18:06:17 by tgrasset          #+#    #+#             */
/*   Updated: 2023/02/28 11:23:51 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(t_redir *redir)
{
	t_redir	*elem;
	t_redir	*next_elem;

	if (redir != NULL)
	{
		elem = redir;
		while (elem != NULL)
		{
			next_elem = elem->next;
			if (elem->name != NULL)
				free(elem->name);
			free(elem);
			elem = next_elem;
		}
	}
}

void	free_comm(t_comm *comm)
{
	t_comm	*elem;
	t_comm	*next_elem;

	if (comm == NULL)
		return ;
	elem = comm;
	while (elem != NULL)
	{
		next_elem = elem->next;
		free_lex(elem->argv);
		close_fds(elem);
		free_redir(elem->redir);
		if (elem->file != NULL)
			free(elem->file);
		free(elem);
		elem = next_elem;
	}
}

int	ft_error(t_sh *sh, int n)
{
	free_lex(sh->env);
	free_all(sh);
	rl_clear_history();
	if (n == 1)
		ft_putendl_fd("Malloc error", 2);
	else if (n == 2)
		ft_putendl_fd("Open error", 2);
	else if (n == 3)
		ft_putendl_fd("Dup error", 2);
	else if (n == 4)
		ft_putendl_fd("Pipe error", 2);
	else if (n == 5)
		ft_putendl_fd("Fork error", 2);
	else if (n == 6)
		ft_putendl_fd("Stat error", 2);
	else if (n == 7)
		ft_putendl_fd("Wait error", 2);
	exit(-1);
}

void	close_fds(t_comm *cmd)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir != NULL)
	{
		if (redir->fd != -42)
			close (redir->fd);
		redir = redir->next;
	}
}

void	free_all(t_sh *sh)
{
	if (sh->comm != NULL)
		free_comm(sh->comm);
	if (sh->buf != NULL)
		free(sh->buf);
	if (sh->lex != NULL)
		free_lex(sh->lex);
	sh->lex = NULL;
	sh->comm = NULL;
	sh->buf = NULL;
}
