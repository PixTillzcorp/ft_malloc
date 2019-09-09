/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-09-06 15:46:13 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-09-09 17:34:32 }--------------------------*/
/******************************************************************************/

#include "malloc.h"
#include <stdio.h>
#include <math.h>

/*
** find freed block
*/

static t_meta	*find_free_block(t_meta *head, size_t size, int tag)
{
	if (head && head->tag >= tag && (head->freed && head->size >= size))
	{
		if (head->size - size > META_SIZE)
			return (split_block(get_split_head(&g_meta, head), head, size, 0));
		return (head);
	}
	else
		return ((!head ? NULL : find_free_block(head->next, size, tag)));
}

/*
** find unused block created with the pregen function.
*/

static t_meta	*find_unused_block(t_meta *head, size_t size, int tag)
{
	if (head && head->tag == tag && !head->used && !head->freed)
		return (head);
	else
		return ((!head ? NULL : find_unused_block(head->next, size, tag)));
}

/*
** Pregenerating PREGEN_NBR of spaces such as there are PREGEN_NBR of TINY
** and PREGEN_NBR of SMALL.
*/

static void	pre_gen(t_meta **ablock, t_meta *head, int index)
{
	if (index == PREGEN_NBR * 2)
		return ;
	if (!head) {
		if (!(*ablock = get_space(NULL, TINY, NULL)))
			return ;
		(*ablock)->pregen = 1;
		return pre_gen(ablock, *ablock, index + 1);
	}
	else {
		get_space(head, (index >= PREGEN_NBR ? SMALL : TINY), NULL);
		head->pregen = 1;
	}
	return pre_gen(ablock, head->next, index + 1);
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
				ft_munmap((void *)head, get_block_size(head) + META_SIZE);
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
** My own malloc function returning a pointer to a mapped zone size long
*/

void	*ft_malloc(size_t size)
{
	t_meta	*block;

	if (!g_meta)
		pre_gen(&g_meta, NULL, 0);
	erase_freed_block(get_head(&g_meta, NULL, 0), 0);
	cat_freed_block(get_head(&g_meta, NULL, 0), NULL);
	if (get_tag(size) <= 1 && (block = find_unused_block(get_head(&g_meta, NULL, 0), size, get_tag(size))))
		block->size = size;
	else if ((block = find_free_block(get_head(&g_meta, NULL, 0), size, get_tag(size))))
		block->freed = 0;
	else if (!(block = get_space(get_head(&g_meta, NULL, 1), size, NULL)))
		return (NULL);
	block->used = 1;
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

// int			main(int argc, char **argv)
// {
// 	// struct rlimit tmp;
// 	char	*str;
// 	char	**test;
// 	int		i;

// 	i = 0;
// 	if (!(test = (char **)ft_malloc(sizeof(char *) * 4)))
// 		ft_putendl("ft_malloc error.");
// 	while (i < 4) {
// 		if (!(test[i] = (char *)ft_malloc(sizeof(char) * (6400 / (int)pow(2, i + 1))))) {
// 			ft_putendl("malloc failed.");
// 			return (1);
// 		}
// 		i++;
// 	}
// 	show_alloc_mem();
// 	if (!(str = (char *)ft_malloc(sizeof(char) * 9000)))
// 		ft_putendl("ft_malloc error.");
// 	i = 0;
// 	while (i < 4) {
// 		ft_free(test[i]);
// 		if (!(test[i] = (char *)ft_malloc(sizeof(char) * (6400 / (int)pow(2, i + 1))))) {
// 			ft_putendl("malloc failed.");
// 			return (1);
// 		}
// 		ft_realloc(test[i], (6400 / (int)pow(2, i + 1)) * 2);
// 		i++;
// 	}
// 	show_alloc_mem();
// 	// if (!(jesus = (char *)ft_realloc(jesus, 1342)))
// 	// 	ft_putendl("ft_realloc error.");
// 	// getrlimit(RLIMIT_DATA, &tmp);
// 	// printf("current = %llu | max = %llu\n", tmp.rlim_cur, tmp.rlim_max);
// 	return 0;
// }

//__________________________________________________________________________________

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

//__________________________________________________________________________________

// int main(int argc, char const *argv[])
// {
// 	char *tmp;
// 	char **str;
// 	int	i;
// 	int len;

// 	len = atoi(argv[1]);
// 	i = 0;
// 	if (!(str = (char **)ft_malloc(sizeof(char *) * len)))
// 		ft_putendl("ft_malloc error.");
// 	while (i < len) {
// 		str[i] = (char *)ft_malloc(sizeof(char) * (arc4random() % 1000));
// 		i++;
// 	}
// 	i = 0;
// 	while (i < len - 1) {
// 		ft_free(str[i++]);
// 	}
// 	if (!(tmp = (char *)ft_malloc(sizeof(char ) * 3000)))
// 		ft_putendl("ft_malloc error.");
// 	show_alloc_mem();
// 	return 0;
// }