#include "malloc.h"

int			main(int argc, char **argv) // not finished
{
	char	*line;
	char	*tmp;

	line = ft_strdup("");
	while ((ft_strcmp(line, "exit")))
	{
		ft_putstr("cmd > ");
		if (line)
			ft_memdel((void *)(&line));
		get_next_line(1, &line);
		if (!strcmp(line, "malloc") || !strcmp(line, "free") || !strcmp(line, "realloc")) {
			tmp = ft_strdup(line);
			ft_memdel((void *)(&line));
			ft_putstr("size > ");
		}
	}
	if (line)
		ft_memdel((void *)(&line));
	return (0);
}