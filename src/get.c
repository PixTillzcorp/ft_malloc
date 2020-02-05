/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heinfalt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 10:11:56 by heinfalt          #+#    #+#             */
/*   Updated: 2020/02/05 10:17:01 by heinfalt         ###   ########.fr       */
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
**----- Date ----------------{ 2019-09-30 12:53:38 }--------------------------**
**----- Author --------------{ PixTillz }-------------------------------------**
**----- Last Modified by ----{ hippolyteeinfalt }-----------------------------**
**----- Last Modified time --{ 2019-10-12 18:47:19 }--------------------------**
********************************************************************************
*/

#include "../includes/malloc.h"

t_page		*get_page(size_t size, char type, size_t nbr)
{
	t_page	*request;
	size_t	alloc;

	alloc = (((BMETA_SIZE + size) * nbr) + PMETA_SIZE);
	if ((request = (t_page *)mmap((g_page ? g_page->last + g_page->size : 0),
	alloc,
	PROT_READ | PROT_WRITE,
	MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	request->next = NULL;
	request->block = NULL;
	request->size = alloc;
	request->remain = alloc - (size_t)((void *)(request + 1) - (void *)request);
	request->type = type;
	if (g_page)
	{
		if (g_page->last)
			g_page->last->next = request;
	}
	else
		g_page = request;
	g_page->last = request;
	return (request);
}

t_block		*get_block(void *addr, size_t size)
{
	t_block *ret;

	if (!addr)
		return (NULL);
	ret = (t_block *)(addr);
	ret->root = (void *)(((uintptr_t)((void *)(ret + 1) + ALIGNEMENT) &
										~((uintptr_t)ALIGNEMENT)));
	ret->size = size;
	ret->freed = 0;
	ret->next = NULL;
	return (ret);
}
