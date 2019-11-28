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

static void destroy_freed(t_block *previous, t_block *bfreed, size_t diff)
{
    t_block *tmp;
    t_block *new;
    void    *addr;
    size_t  to_zero;

    to_zero = (size_t)bfreed->root - (size_t)bfreed + bfreed->size;
    tmp = bfreed->next;
    ft_bzero(previous->root + previous->size, to_zero);
    previous->size += diff;
    addr = previous->root + previous->size;
    if (!(new = get_block(addr, to_zero - diff - BMETA_SIZE)))
        return ;
    new->next = tmp;
    previous->next = new;
}

void        enlarge_block_ts(t_block *block, size_t size)
{
    t_page  *page;
    size_t  diff;

    if (!(page = find_page(g_page, block)))
        return ;
    diff = size - block->size;
    if (!block->next && page->remain > diff)
    {
         block->size += diff;
         page->remain -= diff;
    }
    else if (block->next->freed && block->next->size - BMETA_SIZE > diff)
        destroy_freed(block, block->next, diff);
}

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

static void	*realloc_core(void *ptr, size_t size)
{
	t_block	*ref;
	void	*new_ptr;

	if (!ptr)
        return (malloc_core(size));
	if (!(ref = find_block_addr(&g_page, ptr)))
		return (NULL);
	if (is_ts(ref->size) && is_ts(ref->size) == is_ts(size)) {
        if (ref->size > size && ref->size - size > 0)
            split_block_ts(ref, size);
        else if (ref->size < size)
            enlarge_block_ts(ref, size);
        return (ptr);
    }
	else {
        if (!(new_ptr = malloc_core(size)))
            return (NULL);
        ft_memcpy(new_ptr, ptr, ref->size);
        free_core(ptr);
        return (new_ptr);
    }
}

void		*realloc(void *ptr, size_t size)
{
    void    *ret;

    pthread_mutex_lock(&g_mutex);
    ret = realloc_core(ptr, size);
    pthread_mutex_unlock(&g_mutex);
    return (ret);
}