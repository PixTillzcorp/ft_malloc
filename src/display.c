/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-09-09 17:06:09 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-09-09 18:04:44 }--------------------------*/
/******************************************************************************/

#include "../includes/malloc.h"

static void	put_tag(int tag)
{
	if (tag == SIZE_TINY)
		ft_putendl("TINY");
	if (tag == SIZE_SMALL)
		ft_putendl("SMALL");
	if (tag == SIZE_LARGE)
		ft_putendl("LARGE");
}

static void	put_block(t_meta **ablock, int tag)
{
	t_meta	*head;
	int		flip;

	flip = 0;
	if (!ablock || !(head = *ablock)) {
		ft_putendl("No allocation yet.");
		return ;
	}
	put_tag(tag);
	while (head) {
		if (head->tag == tag && head->used) {
			flip = 1;
			ft_printf("%p - %p : %lu octet\n", (void *)head + META_SIZE, (void *)(head + META_SIZE + head->size), head->size);
		}
		head = head->next;
	}
	if (!flip)
		ft_putendl("No allocation yet.");
}

void		show_alloc_mem()
{
	int		tag;

	tag = 0;
	while (tag < 3)
		put_block(&g_meta, tag++);
	// ---------------------------
	// show_full_mem(&g_meta);
	// ---------------------------
	// put_colored_mem(&g_meta);
}

// void		show_full_mem(t_meta **ablock)
// {
// 	t_meta	*head;

// 	if (!ablock || !(head = *ablock))
// 		return ;
// 	ft_putendl("\033[34m*****************************\033[0m");
// 	while (head) {
// 		put_block_info(head);
// 		head = head->next;
// 	}
// 	ft_putendl("\033[34m*****************************\033[0m");
// }

// void put_block_info(t_meta *block)
// {
// 	ft_printf("tag = %d | freed = %d | used = %d | next = %p | pregen = %d | size = %lu\n", block->tag, block->freed, block->used, block->next, block->pregen, block->size);
// }

// static void put_colored_mem(t_meta **ablock)
// {
// 	t_meta	*head;

// 	if (!ablock || !(head = *ablock))
// 		return ;
// 	while (head)
// 	{
// 		ft_putstr("\033[43m");
// 		ft_putxchar(' ', (META_SIZE / DISPLAY_NBR) + 1); // maybe 2 times larger
// 		ft_putstr((head->used ? "\033[42m" : "\033[49m"));
// 		ft_putstr((head->freed ? "\033[44m" : ""));
// 		ft_putxchar(' ', (head->size > SMALL ? DISPLAY_NBR : head->size / DISPLAY_NBR + 1));
// 		if (head->tag < 2)
// 			ft_putxchar(' ', ((!head->tag ? TINY : SMALL) - head->size) / DISPLAY_NBR);
// 		ft_putstr("\033[0m");
// 		ft_putchar((head->next ? '|' : '\n'));
// 		head = head->next;
// 	}
// }

// static void	ft_show_blocks(t_block **leader)
// {
// 	t_block	*head;

// 	if (!leader || !(head = *leader))
// 		return ;
// 	while (head)
// 	{
// 		if (!(head->used))
// 		{
// 			printf("EMPTY\n");
// 			head = head->next;
// 		}
// 		printf("%p - %p\n", head->data, (void *)((head->data) + head->len));
// 	}
// }

// static void	ft_show_pages(t_page **page, int flip)
// {
// 	t_page	*head;

// 	if (!page || !(head = *page))
// 		return ;
// 	if (flip == TINY)
// 		printf("- TINY -\n");
// 	else
// 		printf("- SMALL -\n");
// 	while (head)
// 	{
// 		ft_show_blocks(&(head->leader));
// 		head = head->next;
// 	}
// }

// void		show_alloc_mem()
// {
// 	ft_show_pages(&(g_malloc.tiny), TINY);
// 	// ft_show_pages(&(g_malloc.small), SMALL);
// 	// printf("- LARGE -\n");
// 	// ft_show_blocks(&(g_malloc.large));
// }
