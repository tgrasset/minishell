/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 15:31:13 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/06 14:50:14 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include "./libft/libft.h"

typedef struct s_redir
{
	char			*name;
	int				output;
	int				doubl;
	int				fd;
	struct s_redir	*next;
}			t_redir;

typedef struct s_comm
{
	char			*file;
	char			**argv;
	t_redir			*redir;
	pid_t			pid;
	struct s_comm	*next;
	int				infile;
	int				outfile;
	int				in_out_fail;
	int				stdout_save;
}			t_comm;

typedef struct s_sh
{
	char	*buf;
	char	**lex;
	t_comm	*comm;
	int		pipe_number;
	int		interrupted_heredoc;
	int		stdin_save;
	char	**env;
}			t_sh;

int		ft_error(t_sh *sh, int n);
char	**setup_env(char **envp);
void	lexing(t_sh *sh);
int		token_count(char *s);
void	free_lex(char **lex);
void	close_fds(t_comm *cmd);
void	pipe_token(t_sh *sh, int *i, int *j);
void	in_redir_token(t_sh *sh, int *i, int *j);
void	out_redir_token(t_sh *sh, int *i, int *j);
void	quote_token(t_sh *sh, int *i, int *j, int type);
void	word_token(t_sh *sh, int *i, int *j);
char	*copy_token(char *s, int flag);
void	parsing(t_sh *sh);
void	free_comm(t_comm *comm);
void	assign_cmd_value(t_comm *new);
int		get_command_name(t_sh *sh, t_comm *new, int i);
void	get_command_args(t_sh *sh, t_comm *new, int i, int j);
void	expand_variables(t_sh *sh, int in_d_quotes, int i, int exp_len);
int		get_var_name_len(char *str);
char	*replace_var(char *str, t_sh *sh, int start, int *exp_len);
char	*replace_2(char *str, char *exp, int start, int end);
void	copy_brackets_var(char *str, char *var_name, int *end, int start);
void	copy_nobrackets_var(char *str, char *var_name, int *end, int start);
char	*replace_by_ret_value(t_sh *sh, char *str, int start, int end);
int		remainder_length(char *str, int end);
void	clean_paired_quotes(t_sh *sh);
void	skip_quotes(char *s, int *i, int type);
int		check_syntax(char **lex, int i);
void	free_all(t_sh *sh);
int		redirections(t_comm *cmd, t_sh *sh);
void	here_doc(t_sh *sh, t_redir *redir);
int		in_hd_delimiter(char *buf, int i, int in_d_quotes);
int		check_if_empty_str(char *str);
int		is_last_redir(t_redir *redir);
void	execution(t_sh *sh, char **env);
void	choose_exec_case(t_sh *sh, t_comm *cmd, int *pipe_fd, char **env);
void	exec_command(t_comm *cmd, t_sh *sh, int *pipe_fd, char **env);
void	exec_command_2(t_comm *cmd, t_sh *sh, char **env);
void	exec_in_pipe(t_sh *sh, int *pipe_fd, t_comm *cmd, char **env);
void	pipe_0(t_sh *sh, int	*pipe_fd, t_comm *cmd);
void	exec_outfile_pipe_0(t_sh *sh, int *pipe_fd, t_comm *cmd, char **env);
char	**split_paths(t_sh *sh, char *temp, char **paths, char *file);
void	command_error(t_sh *sh, char *cmd, int num, char *path);
void	wait_for_children(t_sh *sh);
void	sig_handler_prompt(int signum);
void	sig_handler_command(int signum);
void	check_built_in(t_sh *sh, t_comm *cmd, char **env, int forked);
int		env_built_in(t_comm *cmd);
int		is_unpiped_env_builtin(t_comm *cmd);
void	my_echo(t_sh *sh, t_comm *cmd);
void	my_cd(t_sh *sh, t_comm *cmd, int forked);
int		check_cd_arg(char **argv);
void	cd_replace_env_var(t_sh *sh, char *name, char *value);
void	cd_add_env_var(t_sh *sh, char *name, char *value);
void	cd_error(t_sh *sh, char *dest, int forked, int type);
void	my_pwd(t_sh *sh);
void	my_exit(t_sh *sh, t_comm *cmd, int forked);
void	my_env(t_sh *sh, t_comm *cmd, char **env);
char	*ft_getenv(char *var_name, char **env);
void	my_export(t_sh *sh, t_comm *cmd, int forked);
char	*get_var_name(t_sh *sh, char *arg);
int		invalid_var_name(t_sh *sh, char *arg, int forked);
char	*get_new_value(t_sh *sh, char *arg, char *name);
void	copy_new_var(char *new, char *name, char *value);
void	replace_env(t_sh *sh, char **newenv, char *new);
void	finish_forked_export(t_sh *sh, int forked, int ret_value);
void	my_unset(t_sh *sh, t_comm *cmd, int forked);

#endif
