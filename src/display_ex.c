/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-10-04 15:50:15 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-10-04 15:52:39 }--------------------------*/
/******************************************************************************/

#include "../includes/malloc.h"

static void put_size_clean(size_t size)
{
    if (size < 1000) {
        put_size(size);
        ft_putstr("octet(s)");
    }
    else if (size < 1000000) {
        put_size(size / 1000);
        ft_putstr("Ko");
    }
    else if (size < 1000000000) {
        put_size(size / 1000000);
        ft_putstr("Mo");
    }
    else if (size > 1000000000) {
        put_size(size / 1000000000);
        ft_putstr("Go");
    }
}

static void get_page_type(t_page *page, size_t page_nbr)
{
    if (!page->type)
        ft_putstr("==============\033[38;5;57mLARGE\033[0m=============");
    else if (page->type == TINY)
        ft_putstr("==============\033[38;5;45mTINY\033[0m==============");
    else
        ft_putstr("==============\033[38;5;33mSMALL\033[0m=============");
    ft_putstr(" \033[38;5;242mpage_#");
    put_size(page_nbr);
    ft_putendl("");
    put_hexa_addr((size_t)page, 0);
    ft_putstr(" - ");
    put_hexa_addr((size_t)page + page->size, 0);
    ft_putendl("\033[0m");
}

static size_t get_page_load(t_block *head, size_t octet, size_t page_size)
{
    size_t result;

    if (!head) {
        result = (octet * 100) / page_size;
        if (result <= 50)
            ft_putstr("[\033[38;5;46m");
        else if (result <= 75)
            ft_putstr("[\033[38;5;226m");
        else if (result <= 95)
            ft_putstr("[\033[38;5;202m");
        else if (result <= 100)
            ft_putstr("[\033[38;5;196m");
        if (result)
            put_size(result);
        else
            ft_putstr("<1");
        ft_putstr("%\033[0m] of stored data.\n");
        return (octet);
    }
    if (!head->freed)
        octet += head->size;
    return (get_page_load(head->next, octet, page_size));
}

static size_t get_page_freed(t_block *head, size_t octet, size_t page_size)
{
    size_t result;

    if (!head) {
        if (octet) {
            if ((result = (octet * 100) / page_size) <= 15)
                ft_putstr("[\033[38;5;46m");
            else if (result <= 30)
                ft_putstr("[\033[38;5;226m");
            else if (result <= 50)
                ft_putstr("[\033[38;5;202m");
            else
                ft_putstr("[\033[38;5;196m");
            if (result)
                put_size(result);
            else
                ft_putstr("<1");
        }
        else
            ft_putstr("[\033[38;5;46m0");
        ft_putstr("%\033[0m] of freed data.\n");
        return (octet);
    }
    if (head->freed)
        octet += head->size;
    return (get_page_freed(head->next, octet, page_size));
}

static void get_page_meta(size_t data_stored, size_t remain, size_t page_size)
{
    size_t result;

    result = ((page_size - remain - data_stored) * 100) / page_size;
    if (result >= 50)
        ft_putstr("[\033[38;5;196m");
    else if (result >= 30)
        ft_putstr("[\033[38;5;202m");
    else if (result >= 15)
        ft_putstr("[\033[38;5;226m");
    else
        ft_putstr("[\033[38;5;46m");
    if (result)
        put_size(result);
    else
        ft_putstr("<1");
    ft_putstr("%\033[0m] of meta data.\n");
}

static void get_page_frag_lvl(t_block *head, size_t lvl, int flip)
{
    if (!head) {
        if (lvl > 10)
            ft_putstr("[\033[31mhigh\033[0m]");
        else if (lvl > 5)
            ft_putstr("[\033[33mmedium\033[0m]");
        else
            ft_putstr("[\033[32mlow\033[0m]");
        ft_putstr(" fragmentation lvl.");
        if (lvl) {
            ft_putstr(" \033[38;5;242m* ");
            put_size(lvl);
            ft_putstr(" spot(s) *\033[0m");
        }
        ft_putchar('\n');
        return;
    }
    if (head->freed) {
        if (!flip)
            lvl += 1;
        return (get_page_frag_lvl(head->next, lvl, 1));
    }
    return (get_page_frag_lvl(head->next, lvl, 0));
}

static void get_page_usage(size_t remain, size_t page_size)
{
    size_t result;

    if ((result = 100 - (remain * 100) / page_size) >= 75)
        ft_putstr("[\033[38;5;196m");
    else if (result >= 50)
        ft_putstr("[\033[38;5;202m");
    else if (result >= 25)
        ft_putstr("[\033[38;5;226m");
    else
        ft_putstr("[\033[38;5;46m");
    if (result)
        put_size(result);
    else
        ft_putstr("<1");
    ft_putstr("%\033[0m] of the page used.\n");
    ft_putstr("Remain ");
    put_size_clean(remain);
    ft_putstr(" / ");
    put_size_clean(page_size);
    ft_putendl("");
}

static void	put_alloc_ex(t_page *page, size_t page_nbr)
{
    size_t tmp;

    if (!page)
        return ;
    get_page_type(page, page_nbr);
    tmp = get_page_load(page->block, 0, page->size);
    tmp += get_page_freed(page->block, 0, page->size);
    get_page_meta(tmp, page->remain, page->size);
    ft_putendl("-------------------------");
    get_page_usage(page->remain, page->size);
    ft_putendl("-------------------------");
    get_page_frag_lvl(page->block, 0, 0);
    if (!page->next)
        ft_putendl("\033[38;5;242m###############END##############\033[0m");
    return (put_alloc_ex(page->next, page_nbr + 1));
}

void		show_alloc_mem_ex()
{
    pthread_mutex_lock(&g_mutex);
	if (g_page) {
        ft_putendl("\033[38;5;242m##############START#############\033[0m");
        put_alloc_ex(g_page, 1);
    }
	else
	    ft_putendl("No allocation yet.");
    pthread_mutex_unlock(&g_mutex);
}
