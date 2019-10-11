/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 21:03:38 by jmeier            #+#    #+#             */
/*   Updated: 2019/10/09 00:22:42 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		redirect_error(int error)
{
	if (error)
		return (1);
	return (2);
}

int		failure_error(int error)
{
	return (error);
}