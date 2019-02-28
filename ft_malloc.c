#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void		*ft_malloc_tiny_small()
{
	
}

void		*ft_malloc_large()
{

}

void		*malloc(size_t size)
{
	void	*ret;

	if (size <= 0)
		return (NULL);
	//mutex lock
	if (size < TINY)
		ret = ft_malloc_tiny_small(&g_malloc, TINY, size);
	else if (size < SMALL)
		ret = ft_malloc_tiny_small(&g_malloc, SMALL, size);
	else
		ret = ft_malloc_large(size);
	//mutex unlock
	return (ret);
}


int	main(int argc, char**argv)
{
	return (0);
}
