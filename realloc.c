#include "malloc.h"

static t_meta	*cpy_block(t_meta *block)
{
	void		*cpy_block;

	if (!(cpy_block = ft_malloc(block->size)))
		return (NULL);
	ft_memcpy(cpy_block, (void *)(block + 1), block->size);
	if (munmap((void *)block, get_block_size(block) + META_SIZE) < 0) {
		ft_putendl("Error munmap().");
		return (NULL);
	}
	return (((t_meta *)cpy_block) - 1);
}

static t_meta	*new_space(t_meta *block, size_t size_tmp, size_t size)
{
	t_meta		*rest;

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

static t_meta	*get_split_head(t_meta **ablock, t_meta *focus)
{
	t_meta		*head;

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

void		*split_block(t_meta *head, t_meta *block, size_t size, int flip)
{
	t_meta	*next_block;
	t_meta	*tmp_block;

	next_block = block->next;

	if (!(tmp_block = cpy_block(block)))
		return (NULL);
	if (!(block = new_space(block, tmp_block->size, size)))
		return (NULL);
	block->next->next = (!next_block ? tmp_block : next_block);
	if (head != block)
		head->next = block;
	else
		g_meta = block;
	if (flip)
		ft_memcpy(block + 1, tmp_block + 1, size);
	block->used = 1;
	ft_free(tmp_block + 1);
	return (block + 1);
}

void		*ft_realloc(void *ptr, size_t size)
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
		if (block->size - size > SMALL + META_SIZE)
			return (split_block(get_split_head(&g_meta, block), block, size, 1));
		return (ptr);
	}
	if (!(new_ptr = ft_malloc(size)))
		return (NULL);
	ft_memcpy(new_ptr, ptr, block->size);
	ft_free(ptr);
	return (new_ptr);
}