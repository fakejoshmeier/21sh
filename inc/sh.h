/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/10 17:00:37 by josh              #+#    #+#             */
/*   Updated: 2019/10/09 00:21:06 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include "libft.h"
# include "ft_printf.h"
# include <termios.h>
# include <time.h>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/param.h>
# include <unistd.h>
# include <libgen.h>

# define ENVIRON extern char **environ
# define FUG(a, b) ft_strrchr_ind(a, b)
# define DUG(a, b) ft_strndup(a, b)

/*
** Error Macros
*/

# define ERROR_PROMPT(err, s) ft_printf(BLU"jo.sh"RES": %s: %s\n", err, s)
# define COMMAND_ERR "command not found"
# define PIPE_ERR "pipe failed"
# define DUP_ERR "dup error"
# define FORK_ERR "fork failed"
# define EXECVE_ERR	"execve failed"
# define COMMAND_FAIL "Failed to execute command"
# define NO_FILE_ERR "No such file or directory"
# define IS_DIR_ERR "is a directory"
# define NOT_DIR_ERR "Not a directory"
# define PERM_DENIED "Permission denied"
# define BAD_FD_ERR "Bad file descriptor"
# define OPEN_ERR "open error"
# define AMB_REDIR "ambiguous redirection"

# define SEGFAULT "Segmentation Fault: 11"
# define FLOATING "Floating point exception: 8"
# define BUS_ERROR "Bus error: 10"
# define SIGABORT "Abort: 6"

# define PERMISSIONS S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
# define APPEND O_WRONLY | O_CREAT | O_APPEND
# define TRUNCATE O_WRONLY | O_CREAT | O_TRUNC

typedef struct		s_line
{
	uint8_t			*data;
	size_t			capacity;
	size_t			length;
	size_t			width;
}					t_line;

typedef struct		s_hist
{
	char			*content;
	struct s_hist	*next;
	struct s_hist	*prev;
}					t_hist;

typedef struct		s_cont
{
	char			*var;
	char			*value;
}					t_cont;

typedef struct		s_sh
{
	int				fds[3];
	char			cwd[MAXPATHLEN];
	char			old[MAXPATHLEN];
	char			**bin_arr;
	struct termios	term_settings;
	t_hist			*history;
	t_hist			*curr;
	t_map			trie;
	t_map			builtin;
	t_map			env;
	t_map			path;
}					t_sh;

typedef void		(*t_fptr)(int ac, char **av, t_sh *sh);

int					g_pos;

int					read_line(t_line *line, t_sh *sh);
void				enter_raw_mode(void);

t_line				*init_line(size_t size);
void				line_resize(t_line *line, size_t size);
void				line_push(t_line *line, void *new);
void				*line_pop(t_line *line, void *item);
void				*line_get(t_line *line, size_t idx);

t_hist				*hist_new(char *content);
t_hist				*hist_add(t_hist *hist, char *add);
void				hist_del(t_hist *hist);

void				signal_handler(void);
void				ignore(void);
void				quit(void);
char				**sanitize_av(char **av, int *ac);
char				**map_to_array(t_map *map);
void				setshell(t_sh *sh);
void				bin_helper(t_list **list);
void				wipe_word(int len);
void				clear_line(t_line *line, char *to_push);
t_list				*status_quo(char *path0, char *d_name, int flag);
t_list				*jm_lstnew(char *content, size_t content_size);
char				*chunk(char *str, size_t len);

int					autocomplete(t_line *line, t_sh *sh);
void				handle_write(t_line *line, char in, t_sh *sh);
void				handle_escape(t_line *line, char in, t_sh *sh);
char				*last_piece(char *data);
int					is_op(char *data, int i);

void				update_path(t_sh *sh);
void				lexer_parser(t_line *line, t_sh *sh);

void				b_echo(int ac, char **av, t_sh *sh);
void				b_cd(int ac, char **av, t_sh *sh);
void				b_env(int ac, char **av, t_sh *sh);
void				b_setenv(int ac, char **av, t_sh *sh);
void				b_unsetenv(int ac, char **av, t_sh *sh);
void				b_exit(int ac, char **av, t_sh *sh);

int		redirect_error(int error);
int		failure_error(int error);

#endif
