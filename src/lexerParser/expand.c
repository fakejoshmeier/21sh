/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 16:44:51 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/07 14:53:38 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

/*
** Case 1 for $ Expansion: Braces.  I check for {} and go from there.
** Case 2 for $ Expansion: While the characters are valid or the end of line.
*/

void	trash(t_line *line, char *to_add, int len)
{
	int		i;

	i = 0;
	if (line->data[ft_strlen(((char *)line->data) - 1)] == '\0')
		line_pop(line, NULL);
	while (i < len)
		line_push(line, &to_add[i++]);
}

int		handle_dolla(char *in, t_sh *sh, t_line *line)
{
	t_cont	*tmp;
	char	*str;
	int		i;

	if (!(ft_isalpha(in[0]) || in[0] == '{' || in[0] == '_') ||
		ft_isdigit(in[0]))
		return (0);
	i = 0;
	if (in[0] == '{')
		while (in[i] && in[i] != '}')
			++i;
	else
		while (ft_isalpha(in[i]) || in[i] == '_')
			++i;
	str = in[0] == '{' ? ft_strsub(in, 1, i - 1) : ft_strsub(in, 0, i);
	tmp = ft_map_get(&sh->env, ft_map_hash(&sh->env, str));
	if (tmp)
		trash(line, tmp->value, ft_strlen(tmp->value));
	free(str);
	return (in[0] == '{' ? i + 2 : i + 1);
}

/*
** Basic tilde output.  Only does home for now.
*/

char	*handle_tilde(t_sh *sh)
{
	t_cont	*tmp;

	tmp = ft_map_get(&sh->env, ft_map_hash(&sh->env, "HOME"));
	return (ft_strdup(tmp->value));
}

/*
** Doesn't handle advance tilde substitution.  Will need to rewrite later
** to handle such
*/

char	*expand(char *in, t_sh *sh)
{
	t_line	*line;
	char	*ret;
	int		jump;
	int		len;
	int		i;

	i = 0;
	len = ft_strlen(in);
	line = init_line(1);
	if (len == 1 && in[0] == '~')
		return (handle_tilde(sh));
	while (i < len)
	{
		jump = (in[i] == '$') ? handle_dolla(in + i + 1, sh, line) : 0;
		i += jump;
		if (!jump)
			line_push(line, in + i++);
	}
	line_push(line, "\0");
	ret = ft_strdup((char *)line->data);
	free(line->data);
	free(line);
	return (ret);
}
