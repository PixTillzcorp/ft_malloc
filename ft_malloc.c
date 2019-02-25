#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char**argv)
{
	void *addr;
	// printf("Page size %d Bytes.\n", getpagesize());	4096 or (1 << 12)
	addr = (void *)0x7fdf4cce8000;
	printf("%p --- %s\n", addr, addr);
	return (0);
}
