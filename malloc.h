#ifndef MALLOC_H
# define MALLOC_H

#include "libft/includes/libft.h"

typedef struct			s_malloc
{
	t_page				*tiny;
	t_page				*small;
	t_block				*large;
}						t_malloc;

extern t_malloc			g_malloc;

typedef struct			s_page
{

	t_page				*next;
	t_page				*prev;
}						t_page;

#endif