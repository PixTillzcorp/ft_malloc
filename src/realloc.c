/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-10-01 20:05:46 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-10-03 15:58:35 }--------------------------*/
/******************************************************************************/

#include "../includes/malloc.h"

void		split_block_ts(t_block *block, size_t size)
{
	t_block	*new;
	void	*addr;

	if (!block)
		return ;
	addr = (void *)(((uintptr_t)block->root) + size);
	ft_bzero(addr, block->size - size);
	if (block->next) {
		if (!(new = get_block(addr, block->size - size)))
			return ;
		new->freed = 1;
		new->next = block->next;
		block->next = new;
	}
	block->size = size;
}

void		*realloc(void *ptr, size_t size)
{
	t_block	*ref;
	void	*new_ptr;

	if (!ptr)
		return (malloc(size));
	if (!(ref = find_block_addr(&g_page, ptr)))
	{
		ft_putendl("Trying to realloc a non-allocated space.");
		return (NULL);
	}
    pthread_mutex_lock(&g_mutex);
	if (ref->size >= size && ref->size - size > BMETA_SIZE)
	{
		if (is_ts(ref->size))
			split_block_ts(ref, size);
        pthread_mutex_unlock(&g_mutex);
		return (ptr);
	}
    pthread_mutex_unlock(&g_mutex);
	if (!(new_ptr = malloc(size)))
		return (NULL);
    pthread_mutex_lock(&g_mutex);
	ft_memcpy(new_ptr, ptr, ref->size);
	free(ptr);
    pthread_mutex_unlock(&g_mutex);
	return (new_ptr);
}
