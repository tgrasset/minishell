/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 20:33:56 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/06 14:54:46 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret_val;

int	check_if_empty_str(char *str)
{
	if (str != NULL && str[0] == '\0')
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (1);
	}
	return (0);
}

void	sig_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		g_ret_val = 130;
		close(0);
	}
	if (signum == SIGQUIT)
	{
		printf("\33[2K\r");
		rl_on_new_line();
		rl_redisplay();
	}
}

int	in_hd_delimiter(char *buf, int i, int in_d_quotes)
{
	if (in_d_quotes == 1)
	{
		while (buf[i] != '"')
			i--;
	}
	while (i > 1 && buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n'
		&& buf[i] != '|' && buf[i] != '>' && buf[i] != '<')
		i--;
	while (i > 1 && (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n'))
		i--;
	if (buf[i] == '<' && buf[i - 1] == '<')
		return (1);
	return (0);
}

void	here_doc_loop(char *line, t_redir *redir, t_sh *sh)
{
	signal(SIGINT, sig_handler_heredoc);
	signal(SIGQUIT, sig_handler_heredoc);
	while (1)
	{
		line = readline(">");
		if (g_ret_val == 130 || (line != NULL
				&& ft_strncmp(line, redir->name, ft_strlen(redir->name)) == 0
				&& line[ft_strlen(redir->name)] == '\0'))
		{
			if (g_ret_val == 130)
				sh->interrupted_heredoc = 1;
			free(line);
			break ;
		}
		ft_putendl_fd(line, redir->fd);
		free(line);
	}
	signal(SIGINT, sig_handler_prompt);
	signal(SIGQUIT, sig_handler_prompt);
}

void	here_doc(t_sh *sh, t_redir *redir)
{
	char	*line;

	line = NULL;
	if (is_last_redir(redir) == 0)
		return ;
	g_ret_val = 0;
	redir->fd = open("/tmp/hd", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->fd < 0)
	{
		ft_putendl_fd("msh: heredoc: Permission denied", 2);
		return ;
	}
	dup2(sh->stdin_save, 0);
	here_doc_loop(line, redir, sh);
	close(redir->fd);
	if (g_ret_val != 130)
	{
		redir->fd = open("/tmp/hd", O_RDONLY);
		if (redir->fd < 0)
			ft_error(sh, 2);
		if (dup2(redir->fd, 0) < 0)
			ft_error(sh, 3);
		close(redir->fd);
	}
	unlink("/tmp/hd");
}
