/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-09-09 16:58:02 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-09-09 17:07:51 }--------------------------*/
/******************************************************************************/

#include "malloc.h"

/*
** Concatenate freed block and modify the linked list of blocks
*/

void		cat_block(t_meta *last_block, t_meta *addr, t_meta *next_block)
{
	t_meta	*new_block;
	t_meta	*tmp_addr;
	size_t	size;

	size = 0;
	if (last_block == addr)
		last_block = NULL;
	while (addr && addr != next_block) {
		tmp_addr = addr->next;
		size += addr->size + META_SIZE;
		if (ft_munmap(addr, get_block_size(addr) + META_SIZE) < 0)
			return ;
		addr = tmp_addr;
	}
	new_block = get_space(last_block, size - META_SIZE, (last_block ?\
	last_block->next : g_meta));
	new_block->next = next_block;
	new_block->freed = 1;
}

/*
** Recursively check for freed blocks in a row and concatenate them.
*/

void		cat_freed_block(t_meta *head, t_meta *spot)
{
	if (!head) {
		if (spot)
			cat_block(get_split_head(&g_meta, spot), spot, NULL);
		return ;
	}
	else if (head->freed && (head->next && head->next->freed)) {
		if (spot)
			return (cat_freed_block(head->next, spot));
		else
			return (cat_freed_block(head->next, head));
	}
	else {
		if (spot && head != spot->next)
			cat_block(get_split_head(&g_meta, spot), spot, head);
		return (cat_freed_block(head->next, NULL));
	}
}
