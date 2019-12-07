/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-09-30 18:20:56 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-10-10 15:42:22 }--------------------------*/
/******************************************************************************/

#include "../includes/malloc.h"

static void	erase_page(t_page **afpage, t_page *page)
{
	t_page	*head;
	size_t  tmp;

	if (!afpage || !(head = *afpage) || !page)
		return ;
	if (head == page)
		g_page = page->next;
    else {
        while (head && head->next != page)
            head = head->next;
        if (head)
            head->next = page->next;
        if (page == g_page->last)
            g_page->last = head;
    }
    tmp = page->size;
    ft_bzero((void *)page, tmp);
	if (munmap((void *)page, tmp) < 0)
		return ;
}

static void	what_to_em(t_page *page, t_block *spot, t_block **ablock_start, t_block *end)
{
	t_block	*head;
	size_t	to_em;

	to_em = 0;
	if (!ablock_start || !(head = *ablock_start))
		return ;
	while (head && head != end)
	{
		to_em += (size_t)((size_t)head->root - (size_t)head) + head->size;
		head = head->next;
	}
	ft_bzero((void *)*ablock_start, to_em);
	if (!end) {
		page->remain += to_em;
		(*ablock_start) = end;
	}
	else {
		*ablock_start = get_block((void *)spot, to_em - BMETA_SIZE);
		(*ablock_start)->next = end;
		(*ablock_start)->freed = 1;
	}
}

static void	em_it(t_page *page, t_block **ablock, t_block *spot, t_block *end)
{
	t_block	*head;

	if (!ablock || !(head = *ablock))
		return ;
	if (page->block == spot && !end)
		erase_page(&g_page, page);
	else if (page->block == spot)
		what_to_em(page, spot, &page->block, end);
	else {
		while (head && head->next != spot)
			head = head->next;
		what_to_em(page, spot, &head->next, end);
	}
}

void		em_free_block(t_page *page, t_block *head, t_block *it, int flip)
{
	t_page	*tmp;

	if (!page)
		return ;
	if (!head) {
		tmp = page->next;
		if (flip)
			em_it(page, &(page->block), it, NULL);
		if (!tmp)
			return ;
		return (em_free_block(tmp, tmp->block, NULL, 0));
	}
	else if (head->freed) {
		if (flip)
			return (em_free_block(page, head->next, it, 1));
		else
			return (em_free_block(page, head->next, head, 1));
	}
	else {
		if (flip && it->next != head)
			em_it(page, &(page->block), it, head);
		return (em_free_block(page, head->next, NULL, 0));
	}
}
