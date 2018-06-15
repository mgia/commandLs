#include "ft_ls.h"

int		error(char *s, int err)
{
	if (err == USAGE)
	{
		ft_putstr_fd("ft_ls: illegal option --", 2);
		ft_putchar_fd(*s, 2);
		ft_putstr_fd("\nusage: ls [-lRart] [file ...]\n", 2);
		exit(EXIT_FAILURE);
	}
	else
	{
		ft_putstr_fd("ft_ls: ", 2);
		if (err == ERRNO)
		{
			ft_putstr_fd(s, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
		}
		else
			exit(EXIT_FAILURE);
	}
	return (-1);
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

/*
**
*/

int		get_full_path(char *path, char *name, char full_path[PATH_MAX])
{
	int		i;

	i = -1;
	while (path[++i])
		full_path[i] = path[i];
	if (i && i < PATH_MAX && !(path[0] == '/' && path[1] == '\0'))
		full_path[i++] = '/';
	while (*name && i < PATH_MAX)
		full_path[i++] = *name++;
	if (i < PATH_MAX)
	{
		full_path[i] = '\0';
		return (1);
	}
	else
	{
		errno = ENAMETOOLONG;
		return (0);
	}
}

t_file	*create_file(char *path, char *name, t_stat *stat)
{
	t_file	*file;

	if (!(file = (t_file*)ft_memalloc(sizeof(t_file))) ||
		!(file->name = ft_strdup(name)))
		error(NULL, MALLOC);
	file->mode = stat->st_mode;
	file->st_nlink = stat->st_nlink;
	file->st_uid = stat->st_uid;
	file->st_gid = stat->st_gid;
	file->size = stat->st_size;
	file->st_rdev = stat->st_rdev;
	file->time = stat->st_mtimespec.tv_sec;
	file->ntime = stat->st_mtimespec.tv_nsec;
	// file->st_blocks = stat->st_blocks;
	get_full_path(path, name, file->full_path);
	file->next = NULL;
	return (file);
}

int		add_new_file(char *path, char *name, t_file **lst)
{
	char	full_path[PATH_MAX];
	t_stat	stat;

	if (!(get_full_path(path, name, full_path)))
		return (error(name, 1));
	if (lstat(full_path, &stat) == -1)
		return (-1);
	if (!*lst)
		*lst = create_file(path, name, &stat);
	else
	{
		while ((*lst)->next)
			(*lst) = ((*lst)->next);
		(*lst)->next = create_file(path, name, &stat);
	}
	return (1);
}

t_file	*get_files(int count, char **names)
{
	t_file	*head;
	int		i;

	head = NULL;
	if (!count)
		add_new_file("",".", &head);
	else
	{
		// ft_sort_tab(names, count, &ft_strcmp);
		i = -1;
		while (++i < count)
			if (add_new_file("", names[i], &head) == -1)
				error(names[i], ERRNO);
	}
	return (head);
}

int		main(int ac, char **av)
{
	int		i;
	int		flags;
	t_file	*file_lst;

	i = parse_options(ac, av, &flags);
	file_lst = get_files(ac - i, av + i);
	return (0);
}
