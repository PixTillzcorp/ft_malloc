/*******************************************************************************
**____________________________________________________________________________**
**_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________**
**____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__**
**___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____**
**__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________**
**_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____**
**____________________________________________________________________________**
**                                                                            **
**----- Date ----------------{ 2019-09-24 16:04:55 }--------------------------**
**----- Author --------------{ PixTillz }-------------------------------------**
**----- Last Modified by ----{ hippolyteeinfalt }-----------------------------**
**----- Last Modified time --{ 2019-10-03 19:43:11 }--------------------------**
*******************************************************************************/

#include "../includes/malloc.h"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

static int			check_rlimit(t_page *page, size_t remain, size_t asked)
{
	if (!page)
	{
		if (remain > asked)
			return (1);
		else
		{
			ft_putstr("Not enough resources to alloc:");
			put_size(asked);
			ft_putendl(" octet(s)");
			return (0);
		}
	}
	else
	{
		remain -= page->size - page->remain;
		return (check_rlimit(page->next, remain, asked));
	}
}

int					is_ts(size_t size)
{
	if (size > (size_t)SIZE_SMALL)
		return (LARGE);
	else if (size <= (size_t)SIZE_TINY)
		return (TINY);
	else
		return (SMALL);
}

void				*malloc_core(size_t size)
{
	void			*ret;
	struct rlimit	data;

	if (size <= 0 || getrlimit(RLIMIT_DATA, &data) < 0)
		return (NULL);
	ret = NULL;
	if (check_rlimit(g_page, data.rlim_cur, size + BMETA_SIZE))
	{
		if (is_ts(size))
			ret = alloc_ts(&g_page, size, is_ts(size));
		else
			ret = alloc_large(size);
	}
	return (ret);
}

void				*malloc(size_t size)
{
	void			*ret;

	pthread_mutex_lock(&g_mutex);
	ret = malloc_core(size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
