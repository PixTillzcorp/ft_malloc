/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_ex_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heinfalt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 10:11:39 by heinfalt          #+#    #+#             */
/*   Updated: 2020/02/05 10:15:42 by heinfalt         ###   ########.fr       */
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
**----- Date ----------------{ 2019-10-04 15:50:15 }--------------------------**
**----- Author --------------{ PixTillz }-------------------------------------**
**----- Last Modified by ----{ hippolyteeinfalt }-----------------------------**
**----- Last Modified time --{ 2019-10-04 15:52:39 }--------------------------**
********************************************************************************
*/

#include "../includes/malloc.h"

void		get_page_type(t_page *page, size_t page_nbr)
{
	if (!page->type)
		ft_putstr("==============\033[38;5;57mLARGE\033[0m=============");
	else if (page->type == TINY)
		ft_putstr("==============\033[38;5;45mTINY\033[0m==============");
	else
		ft_putstr("==============\033[38;5;33mSMALL\033[0m=============");
	ft_putstr(" \033[38;5;242mpage_#");
	put_size(page_nbr);
	ft_putendl("");
	put_hexa_addr((size_t)page, 0);
	ft_putstr(" - ");
	put_hexa_addr((size_t)page + page->size, 0);
	ft_putendl("\033[0m");
}

size_t		get_page_load(t_block *head, size_t octet, size_t page_size)
{
	size_t	result;

	if (!head)
	{
		result = (octet * 100) / page_size;
		if (result <= 50)
			ft_putstr("[\033[38;5;46m");
		else if (result <= 75)
			ft_putstr("[\033[38;5;226m");
		else if (result <= 95)
			ft_putstr("[\033[38;5;202m");
		else if (result <= 100)
			ft_putstr("[\033[38;5;196m");
		if (result)
			put_size(result);
		else
			ft_putstr("<1");
		ft_putstr("%\033[0m] of stored data.\n");
		return (octet);
	}
	if (!head->freed)
		octet += head->size;
	return (get_page_load(head->next, octet, page_size));
}

size_t		get_page_freed(t_block *head, size_t octet, size_t page_size)
{
	size_t	result;

	if (!head)
	{
		if (octet)
		{
			if ((result = (octet * 100) / page_size) <= 15)
				ft_putstr("[\033[38;5;46m");
			else if (result <= 30)
				ft_putstr("[\033[38;5;226m");
			else if (result <= 50)
				ft_putstr("[\033[38;5;202m");
			else
				ft_putstr("[\033[38;5;196m");
			(result ? put_size(result) : ft_putstr("<1"));
		}
		else
			ft_putstr("[\033[38;5;46m0");
		ft_putstr("%\033[0m] of freed data.\n");
		return (octet);
	}
	if (head->freed)
		octet += head->size;
	return (get_page_freed(head->next, octet, page_size));
}

void		get_page_meta(size_t data_stored, size_t remain, size_t page_size)
{
	size_t	result;

	result = ((page_size - remain - data_stored) * 100) / page_size;
	if (result >= 50)
		ft_putstr("[\033[38;5;196m");
	else if (result >= 30)
		ft_putstr("[\033[38;5;202m");
	else if (result >= 15)
		ft_putstr("[\033[38;5;226m");
	else
		ft_putstr("[\033[38;5;46m");
	if (result)
		put_size(result);
	else
		ft_putstr("<1");
	ft_putstr("%\033[0m] of meta data.\n");
}

void		get_page_frag_lvl(t_block *head, size_t lvl, int flip)
{
	if (!head)
	{
		if (lvl > 10)
			ft_putstr("[\033[31mhigh\033[0m]");
		else if (lvl > 5)
			ft_putstr("[\033[33mmedium\033[0m]");
		else
			ft_putstr("[\033[32mlow\033[0m]");
		ft_putstr(" fragmentation lvl.");
		if (lvl)
		{
			ft_putstr(" \033[38;5;242m* ");
			put_size(lvl);
			ft_putstr(" spot(s) *\033[0m");
		}
		ft_putchar('\n');
		return ;
	}
	if (head->freed)
	{
		if (!flip)
			lvl += 1;
		return (get_page_frag_lvl(head->next, lvl, 1));
	}
	return (get_page_frag_lvl(head->next, lvl, 0));
}
