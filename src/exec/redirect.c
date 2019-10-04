/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/03 11:39:38 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/03 18:52:45 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
** For redirections into an FD, check the first token in ast->left.  If it's a
** number, it's actually an IO number and I'll work it accordingly.
*/