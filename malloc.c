#include "malloc.h"
#include <stdio.h>

size_t	get_block_size(t_meta *block)
{
	if (block->tag == SIZE_TINY)
		return (TINY);
	else if (block->tag == SIZE_SMALL)
		return (SMALL);
	else
		return (block->size);
}

/*
** get tag of the block
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
** erase freed block
*/

int		erase_freed_block(t_meta *head, int flip)
{
	if (!head)
		return (flip);
	else
	{
		if (head->freed)
		{
			if (erase_freed_block(head->next, 1))
			{
				if (munmap((void *)head, get_block_size(head) + META_SIZE) < 0)
					ft_putendl("Error munmap().");
				return (1);
			}
			else
				return (0);
		}
		else {
			if (erase_freed_block(head->next, 0))
				head->next = NULL;
			return (0);
		}
	}
}

/*
** find freed block
*/

t_meta	*find_free_block(t_meta *head, size_t size, int tag)
{
	if (head && head->tag >= tag && (head->freed && head->size >= size))
		return (head);
	else
		return ((!head ? NULL : find_free_block(head->next, size, tag)));
}

/*
** find unused block
*/

t_meta	*find_unused_block(t_meta *head, size_t size, int tag)
{
	if (head && head->tag == tag && !head->used && !head->freed)
		return (head);
	else
		return ((!head ? NULL : find_unused_block(head->next, size, tag)));
}

/*
** allocate with mmap a space
*/

t_meta	*get_space(t_meta *last_block, size_t size, void *addr)
{
	t_meta	*new_block;
	void	*request;

	new_block = NULL;
	if ((request = mmap(addr, size + META_SIZE, PROT_READ | PROT_WRITE, MAP_ANON |
						MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	new_block = (t_meta *)(request);
	if (last_block)
		last_block->next = new_block;
	new_block->size = size;
	new_block->freed = 0;
	new_block->used = 0;
	new_block->next = NULL;
	new_block->tag = get_tag(size);
	return (new_block);
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

void	pre_gen(t_meta **ablock, t_meta *head, int index)
{
	if (index == PREGEN_NBR * 2)
		return ;
	if (!head) {
		if (!(*ablock = get_space(NULL, TINY, NULL)))
			return ;
		return pre_gen(ablock, *ablock, index + 1);
	}
	else {
		get_space(head, (index >= PREGEN_NBR ? SMALL : TINY), NULL);
	}
	return pre_gen(ablock, head->next, index + 1);
}

/*
** own malloc function
*/

void	*ft_malloc(size_t size)
{
	t_meta	*block;

	if (!g_meta)
		pre_gen(&g_meta, NULL, 0);
	erase_freed_block(get_head(&g_meta, NULL, 0), 0);
	if (!(block = find_free_block(get_head(&g_meta, NULL, 0), size, get_tag(size))))
	{
		if (get_tag(size) <= 1 && (block = find_unused_block(get_head(&g_meta, NULL, 0), size, get_tag(size))))
			block->size = size;
		else
		{
			if (!(block = get_space(get_head(&g_meta, NULL, 1), size, NULL)))
				return (NULL);
		}
		block->used = 1;
	}
	else
		block->freed = 0;
	return (block + 1);
}

// int	count_block(t_meta **ablock)
// {
// 	t_meta *head;
// 	int count;

// 	count = 0;
// 	head = *ablock;
// 	while (head)
// 	{
// 		count++;
// 		head = head->next;
// 	}
// 	return (count);
// }

int			main(int argc, char **argv)
{
	// struct rlimit tmp;
	char	*str;
	char	*jesus;
	char	*is;
	char	*swag;
	int		i;

	i = 0;
	// if (!(str = (char *)ft_malloc(sizeof(char) * 80)))
	// 	ft_putendl("ft_malloc error.");
	if (!(jesus = (char *)ft_malloc(sizeof(char) * 8000)))
		ft_putendl("ft_malloc error.");
	if (!(is = (char *)ft_malloc(sizeof(char) * 800)))
		ft_putendl("ft_malloc error.");
	if (!(swag = (char *)ft_malloc(sizeof(char) * 12000)))
		ft_putendl("ft_malloc error.");
	show_alloc_mem();
	if (!(jesus = (char *)ft_realloc(jesus, 2000)))
		ft_putendl("ft_realloc error.");
	// getrlimit(RLIMIT_DATA, &tmp);
	// printf("current = %llu | max = %llu\n", tmp.rlim_cur, tmp.rlim_max);
	show_alloc_mem();
	ft_free(jesus);
	return 0;
}

// int main(int argc, char const *argv[])
// {
// 	char *str;
// 	char *swag;
// 	char *jesus;
// 	char *is;
// 	char *last;
// 	int	i;

// 	i = 0;
// 	if (!(str = (char *)ft_malloc(sizeof(char) * 658)))
// 		ft_putendl("ft_malloc error.");
// 	if (!(swag = (char *)ft_malloc(sizeof(char) * 142)))
// 		ft_putendl("ft_malloc error.");
// 	if (!(jesus = (char *)ft_malloc(sizeof(char) * 397)))
// 		ft_putendl("ft_malloc error.");
// 	if (!(is = (char *)ft_malloc(sizeof(char) * 703)))
// 		ft_putendl("ft_malloc error.");
// 	show_alloc_mem();
// 	ft_free(jesus);
// 	show_alloc_mem();
// 	if (!(last = (char *)ft_malloc(sizeof(char) * 397)))
// 		ft_putendl("ft_malloc error.");
// 	show_alloc_mem();
// 	ft_printf("str = {%d} | addr / 16 = {%d} | addr %% 16 = {%d}\n", str, (int)(str) / 16, (int)(str) % 16);
// 	ft_printf("str = {%d} | addr / 16 = {%d} | addr %% 16 = {%d}\n", swag, (int)(swag) / 16, (int)(swag) % 16);
// 	ft_printf("str = {%d} | addr / 16 = {%d} | addr %% 16 = {%d}\n", jesus, (int)(jesus) / 16, (int)(jesus) % 16);
// 	ft_printf("str = {%d} | addr / 16 = {%d} | addr %% 16 = {%d}\n", is, (int)(is) / 16, (int)(is) % 16);
// 	return 0;
// }

// int main(int argc, char const *argv[])
// {
// 	char **str;
// 	int	i;
// 	int len;

// 	len = atoi(argv[1]);
// 	i = 0;
// 	if (!(str = (char **)ft_malloc(sizeof(char *) * len)))
// 		ft_putendl("ft_malloc error.");
// 	while (i < len) {
// 		str[i] = (char *)ft_malloc(sizeof(char) * (arc4random() % 1000));
// 		if (!((long int)(str) & 0xFF))
// 			ft_putendl("\033[32mAddress is 8 bytes aligned.\033[0m");
// 		i++;
// 	}
// 	show_alloc_mem();
// 	return 0;
// }