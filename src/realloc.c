/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heinfalt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 10:12:02 by heinfalt          #+#    #+#             */
/*   Updated: 2020/02/05 10:17:33 by heinfalt         ###   ########.fr       */
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
**----- Date ----------------{ 2019-10-01 20:05:46 }--------------------------**
**----- Author --------------{ PixTillz }-------------------------------------**
**----- Last Modified by ----{ hippolyteeinfalt }-----------------------------**
**----- Last Modified time --{ 2019-10-03 15:58:35 }--------------------------**
********************************************************************************
*/

#include "../includes/malloc.h"

static int	destroy_freed(t_block *previous, t_block *bfreed, size_t diff)
{
	t_block	*tmp;
	t_block	*new;
	void	*addr;
	size_t	to_zero;

	to_zero = (size_t)bfreed->next - ((size_t)previous->root + previous->size);
	tmp = bfreed->next;
	new = NULL;
	ft_bzero((void *)((size_t)previous->root + previous->size), to_zero);
	previous->size += diff;
	addr = previous->root + previous->size;
	to_zero = (size_t)tmp - ((size_t)previous->root + previous->size);
	if (to_zero < BMETA_SIZE || !(new = get_block(addr, to_zero - BMETA_SIZE)))
	{
		previous->next = tmp;
		return (1);
	}
	new->freed = 1;
	new->next = tmp;
	previous->next = new;
	return (1);
}

static int	enlarge_block_ts(t_block *block, size_t size)
{
	t_page	*page;
	size_t	diff;

	if (!(page = find_page(g_page, block)))
		return (1);
	diff = size - block->size;
	if (!block->next && page->remain > diff)
	{
		block->size += diff;
		page->remain -= diff;
		return (1);
	}
	else if (block->next->freed && block->next->size - BMETA_SIZE > diff)
		return (destroy_freed(block, block->next, diff));
	else
		return (0);
}

void		split_block_ts(t_block *block, size_t size)
{
	t_block	*new;
	void	*addr;

	if (!block)
		return ;
	addr = (void *)(((size_t)block->root) + size);
	ft_bzero(addr, block->size - size);
	if (block->next)
	{
		if (!(new = get_block(addr, block->size - size - BMETA_SIZE)))
			return ;
		new->freed = 1;
		new->next = block->next;
		block->next = new;
	}
	block->size = size;
}

static void	*realloc_core(void *ptr, size_t size)
{
	t_block	*ref;
	void	*new_ptr;

	if (!ptr)
		return (malloc_core(size));
	if (!(ref = find_block_addr(&g_page, ptr)))
		return (NULL);
	if (is_ts(ref->size) && is_ts(ref->size) == is_ts(size))
	{
		if (ref->size > size && ref->size - size > BMETA_SIZE)
		{
			split_block_ts(ref, size);
			return (ptr);
		}
		else if (ref->size < size)
		{
			if (enlarge_block_ts(ref, size))
				return (ptr);
		}
	}
	if (!(new_ptr = malloc_core(size)))
		return (NULL);
	ft_memcpy(new_ptr, ptr, (ref->size > size ? size : ref->size));
	free_core(ptr);
	return (new_ptr);
}

extern void	*realloc(void *ptr, size_t size)
{
	void	*ret;

	pthread_mutex_lock(&g_mutex);
	ret = realloc_core(ptr, size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
