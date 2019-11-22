/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-10-03 12:52:08 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-10-10 15:55:16 }--------------------------*/
/******************************************************************************/

#include "../includes/malloc.h"

void		put_size(size_t nbr)
{
	if (!nbr)
		return ;
	put_size(nbr / 10);
	ft_putchar((nbr % 10) + 48);
}

void		put_hexa_addr(size_t nbr, int maj)
{
	char		disp;

	if (!nbr) {
		ft_putstr("0x");
		return ;
	}
	put_hexa_addr(nbr / 16, maj);
	if ((disp = nbr % 16) >= 10)
		ft_putchar((disp % 10) + (maj ? 65 : 97));
	else
		ft_putchar(disp + 48);
}

static size_t	put_alloc(t_page *page, t_block *block, size_t octet)
{
	if (!block || block->freed) {
		if (!page->next)
			return (octet);
		return (put_alloc(page->next, page->next->block, octet));
	}
	if (page->block == block)
	{
		if (!page->type)
			ft_putstr("LARGE : ");
		else
			ft_putstr((page->type == TINY ? "TINY : " : "SMALL : "));
		put_hexa_addr((size_t)(page), 1);
		ft_putstr(" - ");
		put_hexa_addr((size_t)(page) + page->size, 1);
		ft_putstr(" -> remain : ");
		put_size(page->remain);
		ft_putchar('\n');
	}
	put_hexa_addr((size_t)(block->root), 1);
	ft_putstr(" - ");
	put_hexa_addr((size_t)(block->root) + block->size, 1);
	ft_putstr(" : ");
	put_size(block->size);
	ft_putstr(" octets\n");
	return (put_alloc(page, block->next, octet + block->size));
}

static void	list_page(t_page **afpage, int flip)
{
	t_page	*head;
	size_t	count;

	if (!afpage || !(head = *afpage)) {
		ft_putchar('0');
		ft_putchar('\n');
		return ;
	}
	count = 0;
	while (head)
	{
		if (flip)
			count += head->size / getpagesize();
		else
			count++;
		head = head->next;
	}
	put_size(count);
	ft_putchar('\n');
}

void		show_alloc_mem()
{
	size_t	total;

    pthread_mutex_lock(&g_mutex);
	if (g_page)
		total = put_alloc(g_page, g_page->block, 0);
	else
		total = 0;
	ft_putstr("Total : ");
	if (total)
		put_size(total);
	else
		ft_putchar('0');
	ft_putchar('\n');
	//----------------------------
	ft_putstr("Pages : ");
	list_page(&g_page, 1);
	//----------------------------
    pthread_mutex_unlock(&g_mutex);
}