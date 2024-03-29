/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump_hexa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heinfalt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 10:11:44 by heinfalt          #+#    #+#             */
/*   Updated: 2020/02/05 10:16:02 by heinfalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
********************************************************************************
**____________________________________________________________________________**
**_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________**
**____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__**
**___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____**
**__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________**
**_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____**
**____________________________________________________________________________**
**                                                                            **
**----- Date ----------------{ 2019-11-21 14:26:38 }--------------------------**
**----- Author --------------{ PixTillz }-------------------------------------**
**----- Last Modified by ----{ hippolyteeinfalt }-----------------------------**
**----- Last Modified time --{ 2019-11-21 14:26:48 }--------------------------**
********************************************************************************
*/

#include "../includes/malloc.h"

#define NBR_COLUMN 16
#define ADDR_LEN 16

static void	put_hexa(size_t value, int flip)
{
	char	disp;

	if (!value)
	{
		if (flip)
			ft_putchar('0');
		return ;
	}
	put_hexa(value / 16, flip);
	if ((disp = value % 16) >= 10)
		ft_putchar((disp % 10) + 97);
	else
		ft_putchar(disp + 48);
}

static void	extract_hexa(void *start, size_t size, char *color)
{
	size_t	i;
	size_t	tmp;

	i = 0;
	put_hexa_addr((size_t)start, 0);
	ft_putchar('\t');
	while (i < size)
	{
		if (!(tmp = (unsigned char)((char *)start)[i++]))
		{
			ft_putstr(color);
			ft_putstr("00\033[0m");
		}
		else
			put_hexa(tmp, (tmp / 16 >= 1 ? 0 : 1));
		if (!(i % NBR_COLUMN) && i && i < size)
		{
			ft_putchar('\n');
			put_hexa_addr((size_t)(start + i), 0);
			ft_putchar('\t');
		}
		else
			ft_putchar(' ');
	}
	ft_putchar('\n');
}

void		dump_hexa(void *ptr)
{
	t_block	*meta;

	pthread_mutex_lock(&g_mutex);
	if (!(meta = find_block_addr(&g_page, ptr)))
		ft_putendl("Trying to dump a non-allocated space.");
	else
	{
		ft_putxchar('-', ADDR_LEN + (NBR_COLUMN * 3));
		ft_putchar('\n');
		if (meta->freed)
			extract_hexa(meta->root, meta->size, "\033[38;5;124m");
		else
			extract_hexa(meta->root, meta->size, "\033[38;5;242m");
		ft_putxchar('-', ADDR_LEN + (NBR_COLUMN * 3));
		ft_putchar('\n');
	}
	pthread_mutex_unlock(&g_mutex);
}
