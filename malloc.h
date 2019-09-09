#ifndef MALLOC_H
# define MALLOC_H

# define TINY 128
# define SMALL 2048
# define PREGEN_NBR 4
# define DISPLAY_NBR 64
# define SIZE_TINY 0
# define SIZE_SMALL 1
# define SIZE_LARGE 2
# define META_SIZE sizeof(t_meta)

#include "libft/includes/libft.h"
#include <sys/resource.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

typedef struct		s_meta
{
	struct s_meta	*next;
	size_t			size;
	char			freed;
	char			used;
	char			tag;
	char			pregen;
}					t_meta;

t_meta				*g_meta;

/*
** malloc.c
*/

int			erase_freed_block(t_meta *head, int flip);
void		*ft_malloc(size_t size);

/*
** free.c
*/

t_meta		*get_addr(void *ptr);
int			check_addr(t_meta **ablock, t_meta *block);
void		ft_free(void *ptr);

/*
** realloc.c
*/

int			ft_munmap(void *ptr, size_t size);
void		*ft_realloc(void *ptr, size_t size);

/*
** cat_block.c
*/

void		cat_block(t_meta *last_block, t_meta *addr, t_meta *next_block);
void		cat_freed_block(t_meta *head, t_meta *spot);

/*
** split_block.c
*/

void		*split_block(t_meta *head, t_meta *block, size_t size, int flip);

/*
** get_functions.c
*/

size_t		get_block_size(t_meta *block);
int			get_tag(size_t size);
t_meta		*get_space(t_meta *last_block, size_t size, void *addr);
t_meta		*get_split_head(t_meta **ablock, t_meta *focus);
t_meta		*get_head(t_meta **ablock, t_meta *block, int flip);

/*
** display.c
*/

void		show_alloc_mem();
void		put_block_info(t_meta *block);

#endif