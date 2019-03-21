/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/10 17:00:37 by josh              #+#    #+#             */
/*   Updated: 2019/03/19 15:21:34 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
# include "libft.h"
# include "ft_printf.h"
# include <termios.h>
# include <time.h>
# include <dirent.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/param.h>
# include <libgen.h>

# define ENVIRON extern char **environ

typedef struct		s_line
{
	uint8_t			*data;
	size_t			capacity;
	size_t			length;
	size_t			width;
}					t_line;

typedef struct		s_cont
{
	char			*var;
	char			*value;
}					t_cont;

typedef struct		s_sh
{
	char			cwd[MAXPATHLEN];
	char			old[MAXPATHLEN];
	char			**bin_arr;
	struct termios	term_settings;
	t_map			builtin;
	t_map			env;
	t_map			path;
}					t_sh;

typedef void		(*t_fptr)(int ac, char **av, t_sh *sh);

#endif
