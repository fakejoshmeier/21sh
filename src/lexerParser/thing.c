/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thing.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 20:56:00 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/03 15:24:29 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include "lexer.h"

void	ft_padding(int padd)
{
	int	i;

	i = 0;
	while (++i < padd)
		write(1, "\t", 1);
}

void	ft_print_node(t_ast *ast, char *side, int lvl)
{
	t_tkn *tmp;

	ft_padding(lvl);
	ft_putstr("** ");
	ft_putstr(side);
	ft_putnbr(lvl);
	ft_putendl(" **");
	ft_padding(lvl);
	tmp = ast->token;
	ft_putstr(MAG);
	while (tmp)
	{
		ft_putstr(tmp->val);
		ft_putstr(" ");
		tmp = tmp->next;
	}
	ft_putendl(RES);
	ft_padding(lvl);
	ft_putendl("************");
}

void	ft_print_ast(t_ast *ast, char *side, int lvl)
{
	if (lvl == 0)
		ft_putendl(MAG"________________ AST ________________"RES);
	if (!ast)
		return ;
	if (ast->left)
		ft_print_ast(ast->left, "left", ++lvl);
	else
		++lvl;
	ft_print_node(ast, side, lvl);
	if (ast->right)
		ft_print_ast(ast->right, "right", lvl--);
	else
		--lvl;
	if (lvl == 0)
		ft_putendl(MAG"_____________________________________"RES);
}
