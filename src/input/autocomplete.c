/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 11:53:30 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/22 18:50:32 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

/*
** Autocomplete notes: The shell autocomplete can detect whether something is an
** operation or operand.
** All operations come from the PATH, all operands come from the directory
** itself.
** Create a circular ll of all the PATH programs/folder progams that match the
** buffer.
*/
/*
void	cycle(t_list *list, char *buf, int flag)
{
	char	c;
	//print the buf
	//print the first completetion
	while (read(STDIN_FILENO, &c, 1))
	{
		if (c == '\t')
		{
			//clear the first one
			
			//cycle
			//print the second
		}
		// Maybe I should set the various behaviours as helper functions so I can call them whenever
		else
		{
			//print the new character whereever
			//
		}
	}
}*/

t_list	*create_autocomplete_ll(char *buf, int buf_len, t_list *bin, int flag)
{
	char	*tmp;
	int		len;
	t_list	*ret;
	t_list	*top;

	if (!buf_len)
		return (bin);
	if (!(tmp = ft_strchr(buf, '/')))
		tmp = buf;
	else
		++tmp;
	len = ft_strlen(tmp);
	while (bin->next)
	{
		
		ret = malloc(sizeof)
	}
	ret->next = top;
	return (bin);
}

t_list	*directory_contents_to_ll(char *buf)
{
	struct dirent	*f;
	struct stat		f_i;
	DIR				*dir;
	t_list			*ret[2];
	char			*path[2];

	path[0] = ft_strchr(buf, '/') ?
		ft_strndup(buf, ft_strrchr_ind(buf, '/') + 1) : ft_strdup("./");
	NULL_GUARD((dir = opendir(path[0])));
	ret[0] = NULL;
	while ((f = readdir(dir)))
	{
		if (f->d_name[0] == '.')
			continue ;
		path[1] = ft_strjoin(path[0], f->d_name);
		stat(path[1], &f_i);
		ret[1] = S_ISDIR(f_i.st_mode) ? ft_lstnew(ft_strjoin(f->d_name, "/"),
		LEN(f->d_name) + 1) : ft_lstnew(ft_strdup(f->d_name), LEN(f->d_name));
		ret[1]->next = ret[0];
		ret[0] = ret[1];
		free(path[1]);
	}
	free(path[0]);
	closedir(dir);
	return (ret[0]);
}

/*
** I have a trie for all the possible programs in the PATH, but if the input
** is not a PATH command, I can rip the dir and get the list of stuff in the
** directory
*/

void	autocomplete(t_line *line, t_sh *sh)
{
	t_list	*bin;
	char	*buf;
	char	*naughtocomplete;
	int		buf_len;
	int		naughtocomplete_len;

	line_push(line, "\0");
	--line->length;
	buf = ft_strrchr((char *)line->data, ' ');
	buf = buf ? buf + 1 : (char *)line->data;
	buf_len = ft_strlen(buf);
	naughtocomplete_len = ft_strlen((char *)line->data) - buf_len;
	if (!naughtocomplete_len)
		bin = ft_map_get(&sh->trie, (uint32_t)buf[0]);
	else
	{
		naughtocomplete = ft_strndup((char *)line->data, naughtocomplete_len);
		bin = directory_contents_to_ll(buf);
	}
	t_list *fug = create_autocomplete_ll(buf, buf_len, bin, naughtocomplete_len);
	for (int i = 0; i < 100; i++)
	{
		if (fug == NULL)
			break ;
		printf("%s\n", fug->content);
		fug = fug->next;
	}
	// cycle(create_autocomplete_ll(buf, bin), buf, naughtocomplete,
	// 	naughtocomplete_len);
}
