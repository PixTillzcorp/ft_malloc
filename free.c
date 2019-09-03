#include "malloc.h"

t_meta			*get_addr(void *ptr)
{
	return (((t_meta *)ptr) - 1);
}

int				check_addr(t_meta **ablock, t_meta *block)
{
	t_meta		*head;

	if (!ablock || !(head = *ablock))
		return (0);
	while (head)
	{
		if (head == block)
			return (1);
		head = head->next;
	}
	return (0);
}

void			ft_free(void *ptr)
{
	t_meta		*block;

	if (!ptr)
		return ;
	if (!check_addr(&g_meta, (block = get_addr(ptr)))) {
		ft_putendl("Trying to free a non-allocated space.");
		ft_printf("addr = %p\n", block);
		return ;
	}
	if (block->freed != 0) {
		ft_putendl("Double free.");
		return ;
	}
	else {
		block->used = 0;
		block->freed = 1;
	}
	erase_freed_block(get_head(&g_meta, NULL, 0), 0);
}