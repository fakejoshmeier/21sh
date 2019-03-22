/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thing.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 06:10:15 by jmeier            #+#    #+#             */
/*   Updated: 2019/03/15 08:09:40 by jmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "/usr/include/curses.h"
#include "/usr/include/term.h"
#include "/usr/include/termcap.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(void)
{
	char buf[1024];
	char buf2[30];
	char *ap = buf2;
	char *clearstr, *gotostr, *standstr, *stendstr;

	tgetent(buf, getenv("TERM"));

	clearstr = tgetstr("cl", &ap);
	gotostr = tgetstr("cm", &ap);
	standstr = tgetstr("so", &ap);
	stendstr = tgetstr("se", &ap);

	fputs(clearstr, stdout);
	fputs(tgoto(gotostr, 20, 10), stdout);
	printf("Hello, ");
	fputs(standstr, stdout);
	printf("world");
	fputs(stendstr, stdout);
	putchar('!');
	return (0);
}

