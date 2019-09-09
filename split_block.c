/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-09-09 15:47:26 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-09-09 17:35:12 }--------------------------*/
/******************************************************************************/

#include "malloc.h"

/*
** This function is used when splitting from realloc
** It copies the original block at g_meta's end.
*/

static t_meta	*cpy_block(t_meta *block)
{
	void		*cpy_block;

	if (!(cpy_block = ft_malloc(block->size)))
		return (NULL);
	ft_memcpy(cpy_block, (void *)(block + 1), block->size);
	return (((t_meta *)cpy_block) - 1);
}

/*
** Creates two space at the original addr pointed by "block" with the size
** of the original one.
*/

static t_meta	*new_space(t_meta *block, size_t size_tmp, size_t size)
{
	if (!(block = get_space(NULL, size, (void *)block))) {
		ft_putendl("Error mmap().");
		return (NULL);
	}
	if (!(block->next = get_space(NULL, size_tmp - (size + META_SIZE),\
	((void *)(block + 1) + block->size)))) {
		ft_putendl("Error mmap().");
		return (NULL);
	}
	block->next->used = 0;
	block->next->freed = 1;
	return (block);
}

/*
** Munmap the last block and link the new spaces to g_meta.
*/

extern void	*split_block(t_meta *head, t_meta *block, size_t size, int flip)
{
	t_meta	*next_block;
	t_meta	*tmp_block;
	size_t	b_size;

	next_block = block->next;
	if (flip) {
		if (!(tmp_block = cpy_block(block)))
			return (NULL);
	}
	if (ft_munmap((void *)block, (b_size = get_block_size(block)) + META_SIZE) < 0)
		return (NULL);
	if (!(block = new_space(block, b_size, size)))
		return (NULL);
	block->next->next = (!next_block && flip ? tmp_block : next_block);
	if (head != block)
		head->next = block;
	else
		g_meta = block;
	if (flip)
	{
		ft_memcpy(block + 1, tmp_block + 1, size);
		ft_free(tmp_block + 1);
	}
	block->used = 1;
	return (block);
}
