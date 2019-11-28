/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-09-30 14:27:10 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-10-08 19:27:51 }--------------------------*/
/******************************************************************************/

#include "../includes/malloc.h"

t_page          *find_page(t_page *head, t_block *block)
{
    if (!head)
        return (NULL);
    else {
        if (find_in_page(&head->block, block->root))
            return (head);
        else
            return (find_page(head->next, block));
    }
}

t_block	        *find_in_page(t_block **afblock, void *ptr)
{
	t_block		*head;

	if (!afblock || !(head = *afblock))
		return (NULL);
	while (head)
	{
		if (head->root == ptr)
			return (head);
		head = head->next;
	}
	return (NULL);
}

t_block			*find_block_addr(t_page **afpage, void *ptr)
{
	t_block		*ret;
	t_page		*head;

	if (!afpage || !(head = *afpage))
        return (NULL);
	while (head)
	{
		if ((ret = find_in_page(&(head->block), ptr)))
            return (ret);
		else
			head = head->next;
	}
	return (NULL);
}

t_block			*find_free_block(t_page *page, t_block *head, size_t size)
{
	if (!page)
		return (NULL);
	if (!head || page->type != is_ts(size))
		if (page->next)
			return (find_free_block(page->next, page->next->block, size));
		else
			return (NULL);
	else if (head->freed && head->size >= size) {
		if (head->size - size > BMETA_SIZE)
			split_block_ts(head, size);
		head->freed = 0;
		return (head);
	}
	else
		return (find_free_block(page, head->next, size));
}
