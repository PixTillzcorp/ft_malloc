/*******************************************************************************
**____________________________________________________________________________**
**_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________**
**____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__**
**___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____**
**__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________**
**_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____**
**____________________________________________________________________________**
**                                                                            **
**----- Date ----------------{ 2019-09-30 12:52:46 }--------------------------**
**----- Author --------------{ PixTillz }-------------------------------------**
**----- Last Modified by ----{ hippolyteeinfalt }-----------------------------**
**----- Last Modified time --{ 2019-10-10 15:50:16 }--------------------------**
*******************************************************************************/

#include "../includes/malloc.h"

static t_block	*add_block(t_page *page, t_block **ablock, size_t size)
{
	t_block		*head;
	t_block		*new;
	void		*addr;

	if (!ablock)
		return (NULL);
	if (!(head = *ablock))
		addr = (void *)(page + 1);
	else
	{
		while (head->next)
			head = head->next;
		addr = (void *)((size_t)head->root - (size_t)head + (head->size));
		addr = (void *)((size_t)head + (size_t)addr);
	}
	if (!(new = get_block(addr, size)))
		return (NULL);
	else if (head)
		head->next = new;
	else
		page->block = new;
	return (new);
}

void			*alloc_large(size_t size)
{
	t_block		*block;
	t_page		*new;

	if (!(new = get_page(size, LARGE, 1)))
	{
		ft_putstr("Error at page creation for size: ");
		put_size(size);
		ft_putendl("");
		return (NULL);
	}
	block = get_block((void *)(new + 1), size);
	new->block = block;
	new->remain -= (size_t)(block->root - (void *)block) + block->size;
	return (block->root);
}

void			*alloc_ts(t_page **afpage, size_t size, int tag)
{
	t_block		*new;
	t_page		*head;

	if (!afpage)
		return (NULL);
	if (!(head = *afpage))
		head = get_page((tag == 1 ? SIZE_TINY : SIZE_SMALL), tag, PREGEN);
	if ((new = find_free_block(g_page, g_page->block, size)))
		return (new->root);
	while (head)
	{
		if (head->type == tag && head->remain >= size + BMETA_SIZE)
		{
			if (!(new = add_block(head, &(head->block), size)))
				return (NULL);
			head->remain -= (size_t)(new->root - (void *)(new)) + new->size;
			return (new->root);
		}
		if (!head->next && tag == 1)
			head->next = get_page(SIZE_TINY, tag, PREGEN);
		else if (!head->next)
			head->next = get_page(SIZE_SMALL, tag, PREGEN);
		head = head->next;
	}
	return (NULL);
}
