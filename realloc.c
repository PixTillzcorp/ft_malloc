/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-09-09 15:49:01 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-09-09 17:41:28 }--------------------------*/
/******************************************************************************/

#include "malloc.h"

int		ft_munmap(void *ptr, size_t size)
{
	if (munmap(ptr, size) < 0) {
		ft_putendl("Error munmap().");
		ft_printf("addr err = %p\n", ptr);
		return (-1);
	}
	return (0);
}

void		*realloc(void *ptr, size_t size)
{
	t_meta	*block;
	void	*new_ptr;

	if (!ptr)
		return (malloc(size));
	if (!check_addr(&g_meta, (block = get_addr(ptr)))) {
		ft_putendl("Trying to realloc a non-allocated space.");
		return (NULL);
	}
	if (block->size >= size) {
		if (block->size - size > META_SIZE) {
			block = split_block(get_split_head(&g_meta, block), block, size, 1);
			return (block + 1);
		}
		return (ptr);
	}
	if (!(new_ptr = malloc(size)))
		return (NULL);
	ft_memcpy(new_ptr, ptr, block->size);
	free(ptr);
	return (new_ptr);
}