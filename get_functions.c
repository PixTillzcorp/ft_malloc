/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-09-09 17:04:32 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-09-09 17:05:32 }--------------------------*/
/******************************************************************************/

#include "malloc.h"

/*
** get block's size from tag
*/

size_t	get_block_size(t_meta *block)
{
	if (block->tag == SIZE_TINY) 
		return ((block->pregen ? TINY : block->size));
	else if (block->tag == SIZE_SMALL)
		return ((block->pregen ? SMALL : block->size));
	else
		return (block->size);
}

/*
** get block's tag from a size
*/

int		get_tag(size_t size)
{
	if (size <= TINY)
		return (SIZE_TINY);
	else if (size <= SMALL)
		return (SIZE_SMALL);
	else
		return (SIZE_LARGE);
}

/*
** allocate with mmap a space of (size + META_SIZE)
*/

t_meta	*get_space(t_meta *last_block, size_t size, void *addr)
{
	t_meta	*new_block;
	void	*request;

	new_block = NULL;
	if ((request = mmap(addr, size + META_SIZE, PROT_READ | PROT_WRITE, MAP_ANON |
						MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
		return (NULL);
	}
	new_block = (t_meta *)(request);
	if (last_block)
		last_block->next = new_block;
	new_block->size = size;
	new_block->freed = 0;
	new_block->used = 0;
	new_block->pregen = 0;
	new_block->next = NULL;
	new_block->tag = get_tag(size);
	return (new_block);
}

/*
** get the head of the block pointing in ->next the block : focus
** if head == focus, the head is returned.
*/

t_meta		*get_split_head(t_meta **ablock, t_meta *focus)
{
	t_meta	*head;

	if (!ablock || !(head = *ablock))
		return (NULL);
	if (head == focus)
		return (head);
	else {
		while (head && head->next && head->next != focus)
			head = head->next;
	}
	return (head);
}

/*
** get the head of block's list with a flip : the first or the last
** if block is not NULL and flip is true, get the head on block
*/

t_meta		*get_head(t_meta **ablock, t_meta *block, int flip)
{
	t_meta	*head;

	if (!ablock || !(head = *ablock))
		return (NULL);
	else {
		if (flip) {
			while (head->next)
			{
				if (block == head)
					return (head);
				head = head->next;
			}
		}
		return (head);
	}
}
