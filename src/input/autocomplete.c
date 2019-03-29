/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 11:53:30 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/24 00:24:07 by jmeier           ###   ########.fr       */
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

void	handle_switch(t_line *line, t_list *list, char in, t_sh *sh)
{
	char	*str;

	str = list->content;
	while (*str)
		line_push(line, &(*str++));
	if (in == sh->term_settings.c_cc[VERASE] && line->length)
		handle_write(line, in, 1);
	//else if ((int)in == 27)
	//	handle_escape(line, );
	else if (ft_isprint(in))
		handle_write(line, in, 0);
	//cleanlisthere
}

void	cycle(t_list *list, t_line *line, char *naughtocomplete, t_sh *sh)
{
	char	c;

	c = '\0';
	if (!line->data)
		return ;
	line->length = 0;
	if (naughtocomplete)
		while (*naughtocomplete)
			line_push(line, &(*naughtocomplete++));
	write(STDOUT_FILENO, list->content, list->content_size);
	while (list)
	{
		if (read(STDIN_FILENO, &c, 1))
		{
			if (c == '\t')
			{
				//clear the first one
				list = list->next;
				write(STDOUT_FILENO, list->content, list->content_size);
			}
			else
				handle_switch(line, list, c, sh);
				break ;
		}
	}
}

t_list	*create_autocomplete_ll(char *buf, t_list *bin)
{
	char	*tmp;
	int		len;
	t_list	*ret;
	t_list	*top;
	t_list	*trash;

	tmp = ft_strchr(buf, '/') ? ft_strchr(buf, '/') + 1 : buf;
	len = ft_strlen(tmp);
	top = NULL;
	printf("CREATING THE FUCKING LIST!!!!!!!!!!\n");
	while (bin)
	{
		if (ft_strnequ(tmp, bin->content, len))
		{
			printf("SHIT, I'M ALLOCATING STUFF WATHC THE FUCK OUT\n");
			ret = (t_list *)malloc(sizeof(t_list));
			ret->content_size = bin->content_size;
			ret->content = ft_strdup(bin->content);
			ret->next = top;
			if (!top)
				trash = ret;
			top = ret;
		}
		bin = bin->next;
	}
	trash->next = top;
	return (top);
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
		ret[1] = S_ISDIR(f_i.st_mode) ? ft_lstnew(ft_strcat(f->d_name, "/"),
		LEN(f->d_name) + 1) : ft_lstnew(f->d_name, LEN(f->d_name));
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

int		autocomplete(t_line *line, t_sh *sh)
{
	t_list	*bin;
	char	*buf;
	char	*naughtocomplete;
	int		naughtocomplete_len;

	line_push(line, "\0");
	--line->length;
	buf = ft_strrchr((char *)line->data, ' ');
	buf = buf ? buf + 1 : (char *)line->data;
	naughtocomplete_len = ft_strlen((char *)line->data) - ft_strlen(buf);
	bin = naughtocomplete_len ? directory_contents_to_ll(buf) :
		ft_map_get(&sh->trie, (uint32_t)buf);
	if (!bin)
		return (FALSE);
	naughtocomplete = ft_strndup((char *)line->data, naughtocomplete_len);
	//bin[1] = create_autocomplete_ll(buf, bin[0]);
	cycle(create_autocomplete_ll(buf, bin), line, naughtocomplete, sh);
	if (naughtocomplete_len)
	{
		clean_bin(&bin);
		free(naughtocomplete);
	}
	return (FALSE);
}
