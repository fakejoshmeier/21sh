/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/07 22:35:43 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/21 13:55:13 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	hashout_bin(t_map *m_p, struct dirent *f, char *content)
{
	free(ft_map_remove(m_p, ft_map_hash(m_p, f->d_name)));
	ft_map_set(m_p, ft_map_hash(m_p, f->d_name), ft_strdup(content));
}

/*
** I should be shot for this joke alone
*/

void	trie_larson(t_map *trie, char *bin, size_t len)
{
	t_list	*new;
	t_list	*ptr;
	char	c;

	c = bin[0];
	new = (t_list *)malloc(sizeof(t_list));
	new->content = ft_strdup(bin);
	new->content_size = len;
	new->next = (ptr = ft_map_remove(trie, (uint32_t)c)) ? ptr : NULL;
	ft_map_set(trie, (uint32_t)c, new);
}

void	find_binary(DIR *dir, char *path, t_sh *sh)
{
	struct dirent	*f;
	struct stat		file_info;
	unsigned		len;
	char			*path_mod;
	char			*tmp;

	len = ft_strlen(path);
	path_mod = len > 1 && path[len - 1] != '/' ? ft_strjoin(path, "/") :
		ft_strdup(path);
	while ((f = readdir(dir)))
	{
		tmp = ft_strjoin(path_mod, f->d_name);
		if (f->d_name[0] != '.' || !access(tmp, X_OK))
		{
			stat(tmp, &file_info);
			if (S_ISREG(file_info.st_mode))
			{
				hashout_bin(&sh->path, f, tmp);
				trie_larson(&sh->trie, f->d_name, ft_strlen(f->d_name));
			}
		}
		free(tmp);
	}
	free(path_mod);
}

void	update_path(t_sh *sh)
{
	t_cont		*env_path;
	char		**path_arr;
	DIR			*dir;
	int			i;

	if (!(env_path = ft_map_get(&sh->env, ft_map_hash(&sh->env, "PATH"))))
		return ;
	path_arr = ft_strsplit(env_path->value, ':');
	i = 0;
	while (path_arr[i])
	{
		if ((dir = opendir(path_arr[i])))
		{
			find_binary(dir, path_arr[i], sh);
			closedir(dir);
		}
		free(path_arr[i++]);
	}
	free(path_arr);
}
