#include "ft_ls.h"

int		error(char *s, int err)
{
	if (err == USAGE)
	{
		ft_printf("ls: illegal option -- %c\n", *s);
		ft_printf("usage: ls [-lRart] [file ...]\n");
		exit(EXIT_FAILURE);
	}
	if (err == ERRNO)
	{
		ft_printf("ls: %s\n", strerror(errno));
	}
	return (0);
}

void	parse_option(char *s, int *flags)
{
	int		i;
	while (*(++s))
	{
		if ((i = ft_strchri("lRart", *s)) == -1)
			error(s, USAGE);
		*flags |= (1 << i);
	}
}

int		parse_options(int ac, char **av, int *flags)
{
	int		i;

	*flags = 0;
	i = 0;
	while (++i < ac && av[i][0] == '-' && av[i][1])
		parse_option(av[i], flags);
	return (i);
}

int		main(int ac, char **av)
{
	int		i;
	int		flags;

	i = parse_options(ac, av, &flags);
	return (0);
}
