#ifndef MALLOC_H
# define MALLOC_H

# define TINY 1
# define SMALL 2
# define LARGE 0
# define PREGEN 100
# define DISPLAY 64
# define SIZE_TINY getpagesize() / 32
# define SIZE_SMALL getpagesize() / 2
# define BMETA_SIZE (sizeof(t_block) + ALIGNEMENT)
# define PMETA_SIZE sizeof(t_page)
# define ALIGNEMENT 7

#include "../libft/includes/libft.h"
#include <sys/resource.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>

typedef struct		    s_block
{
	struct s_block	    *next;
	void			    *root;
	size_t			    size;
	char			    freed;
}					    t_block;

typedef struct		    s_page
{
	struct s_page	    *next;
	struct s_page	    *last;
	t_block			    *block;
	size_t			    size;
	size_t			    remain;
	char			    type;
}					    t_page;

/*
**	alloc.c
*/

void				    *alloc_large(size_t size);
void				    *alloc_ts(t_page **afpage, size_t size, int tag);

/*
**	display.c
*/

void				    show_alloc_mem();
void			        put_hexa_addr(size_t nbr, int maj);
void		            put_size(size_t nbr);

/*
**	display_ex.c
*/

void				    show_alloc_mem_ex();


/*
**	erase_merge.c
*/

void				    em_free_block(t_page *page, t_block *head, t_block *it,
					    int flip);

/*
**	find.c
*/

t_block				    *find_block_addr(t_page **afpage, void *ptr);
t_block				    *find_free_block(t_page *page, t_block *head,
                        size_t size);

/*
**	free.c
*/

void				    free(void *ptr);

/*
**	get.c
*/

t_page				    *get_page(size_t size, char type, size_t nbr);
t_block				    *get_block(void *addr, size_t size);

/*
**	malloc.c
*/

int					    is_ts(size_t size);
void				    *malloc(size_t size);

/*
**	realloc.c
*/

void				    split_block_ts(t_block *block, size_t size);
void				    *realloc(void *ptr, size_t size);

/*
**	calloc.c
*/

void                    *calloc(size_t count, size_t size);

/*
**	dump_hexa.c
*/

void                    dump_hexa(void *ptr);

t_page				    *g_page;
extern pthread_mutex_t	g_mutex;

#endif