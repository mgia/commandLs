#include "ft_ls.h"

/*
** Error handler
*/

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

/*
** Sorting function
*/
int		ft_sort_tab(char **tab, int size, int (*f)(const char*, const char*))
{
	int		i;
	int		j;
	char	*tmp;

	j = 0;
	while (j < size)
	{
		i = j - 1;
		tmp = tab[j];
		while (i >= 0 && f(tab[i], tmp) > 0)
		{
			tab[i + 1] = tab[i];
			i--;
		}
		tab[i + 1] = tmp;
		j++;
	}
	return (1);
}

/*
** Parse flags
*/

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
** Create file list
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
	file->st_blocks = stat->st_blocks;
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
			lst = &((*lst)->next);
		(*lst)->next = create_file(path, name, &stat);
	}
	return (1);
}

/*
** Initialize args as list
*/

t_file	*init_list(int count, char **names)
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

int		ft_ascii(t_file *a, t_file *b)
{
	return (ft_strcmp(a->name, b->name) < 0);
}

int		ft_time(t_file *a, t_file *b)
{
	return (a->time > b->time || (a->time == b->time
		&& a->ntime > b->ntime));
}

t_file	*join(t_file *a, t_file *b, int (*f)(t_file*, t_file*))
{
	t_file	*result;

	result = NULL;
	if (!a)
		return (b);
	else if (!b)
		return (a);
	if (f(a, b))
	{
		result = a;
		result->next = join(a->next, b, f);
	}
	else
	{
		result = b;
		result->next = join(a, b->next, f);
	}
	return (result);
}

void	split_list(t_file *source, t_file** frontRef, t_file** backRef)
{
	t_file	*fast;
	t_file	*slow;

	slow = source;
	fast = source->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}

void	mergeSort(t_file **headRef, int (*f)(t_file*, t_file*))
{
	t_file*		head = *headRef;
	t_file*		a;
	t_file*		b;

	if (!head || !head->next)
		return ;
	split_list(head, &a, &b);
	mergeSort(&a, f);
	mergeSort(&b, f);
	*headRef = join(a, b, f);
}

void	reverse_list(t_file **list)
{
	t_file *prev;
	t_file *curr;
	t_file *next;

	prev = NULL;
	curr = *list;
	next = NULL;
	while (curr)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	*list = prev;
}

void	sort_list(t_file **list, int flags)
{
	mergeSort(list, &ft_ascii);
	if (flags & LS_T)
		mergeSort(list, &ft_time);
	if (flags & LS_R)
		reverse_list(list);
}

typedef struct s_point
{
	int		x;
	int		y;
}				t_point;

/*
** Print functions
*/

int		ft_max(int a, int b)
{
	return (a > b ? a : b);
}

void	get_dimensions(t_file *list, t_point *max, t_point *dims)
{
	struct ttysize	ts;

	ioctl(0, TIOCGWINSZ, &ts);
	max->y = 0;
	max->x = 0;
	dims->y = 0;
	while (list)
	{
		max->x = ft_max(ft_strlen(list->name), max->x);
		dims->y++;
		list = list->next;
	}
	max->x += 8;
	dims->x = ts.ts_cols / max->x;
	dims->y = (dims->y / dims->x) + (dims->y % dims->x ? 1 : 0);
}

void	print_dimensions(t_file *list, t_point *max, t_point *dims)
{
	int		count;
	int		x;
	int		y;
	t_file	*next;

	count = dims->y;
	while (list && count--)
	{
		next = list->next;
		x = dims->x;
		while (list && x--)
		{
			ft_printf("%s", list->name);
			ft_putnchar(' ', ft_max(max->x - ft_strlen(list->name), 0));
			y = dims->y;
			while (list && y--)
				list = list->next;
		}
		ft_putchar('\n');
		list = next;
	}
}

void	print_simple(t_file *list)
{
	t_point		max;
	t_point		dims;

	get_dimensions(list, &max, &dims);
	print_dimensions(list, &max, &dims);
}


/*
** size[0] = st_blocks
** size[1] =
**
**
**
*/

char	get_file_type(int mode)
{
	mode = (mode & S_IFMT);
	if (S_ISREG(mode))
		return ('-');
	else if (S_ISDIR(mode))
		return ('d');
	else if (S_ISLNK(mode))
		return ('l');
	else if (S_ISBLK(mode))
		return ('b');
	else if (S_ISCHR(mode))
		return ('c');
	else if (S_ISSOCK(mode))
		return ('s');
	else if (S_ISFIFO(mode))
		return ('p');
	return ('-');
}

void	print_chmod(t_file **file)
{
	char	chmod[11];
	int		mode;

	mode = (*file)->mode;
	chmod[0] = get_file_type((*file)->mode);
	chmod[1] = (S_IRUSR & mode) ? 'r' : '-';
	chmod[2] = (S_IWUSR & mode) ? 'w' : '-';
	chmod[3] = (S_IXUSR & mode) ? 'x' : '-';
	chmod[4] = (S_IRGRP & mode) ? 'r' : '-';
	chmod[5] = (S_IWGRP & mode) ? 'w' : '-';
	chmod[6] = (S_IXGRP & mode) ? 'x' : '-';
	chmod[7] = (S_IROTH & mode) ? 'r' : '-';
	chmod[8] = (S_IWOTH & mode) ? 'w' : '-';
	chmod[9] = (S_IXOTH & mode) ? 'x' : '-';
	chmod[10] = '\0';
	ft_printf("%s", chmod);
}

void	print_links(t_file **file, int size)
{
	int amt = ft_max(size - ft_nbrlen((*file)->st_nlink), 0) + 2;

	ft_putnchar(' ', amt);
	ft_printf("%hu", (*file)->st_nlink);
}

void	print_id(t_file **file, int uid, int gid)
{
	ft_putchar(' ');
	ft_printf("%s", getpwuid((*file)->st_uid)->pw_name);
	ft_putnchar(' ', ft_max(uid - ft_strlen(getpwuid((*file)->st_uid)->pw_name), 0) + 2);
	ft_printf("%s", getgrgid((*file)->st_gid)->gr_name);
	ft_putnchar(' ', ft_max(gid - ft_strlen(getgrgid((*file)->st_gid)->gr_name), 0));
}

void	print_size(t_file **file, int size)
{
	ft_putnchar(' ', ft_max(size - ft_nbrlen((*file)->size), 0) + 2);
	ft_printf("%lld", (*file)->size);
}

void	print_time(t_file **file)
{
	char	*s;

	s = ctime(&((*file)->time)) + 4;
	ft_printf(" %.12s ", s);
}

void	print_name(t_file **file)
{
	char buf[NAME_MAX + 1];
	if (S_ISLNK((*file)->mode))
	{
		ft_bzero(buf, NAME_MAX + 1);
		readlink((*file)->full_path, buf, NAME_MAX);
		ft_printf("%s -> %s", (*file)->name, buf);
	}
	else
		ft_printf("%s", (*file)->name);
}

void	print_item(t_file **file, int size[7])
{
	print_chmod(file);
	print_links(file, size[1]);
	print_id(file, size[2], size[3]);
	print_size(file, size[4]);
	print_time(file);
	print_name(file);
	ft_putchar('\n');
}

int	get_block_size(t_file *list, int size[7])
{
	int total = 0;
	size[0] = 0;
	while (list)
	{
		size[1] = ft_max(ft_nbrlen(list->st_nlink), size[1]);
		size[2] = ft_max(ft_strlen(getpwuid(list->st_uid)->pw_name), size[2]);
		size[3] = ft_max(ft_strlen(getgrgid(list->st_gid)->gr_name), size[3]);
		size[4] = ft_max(ft_nbrlen(list->size), size[4]);
		total += list->st_blocks;
		list = list->next;
	}
	return total;
}

void	print_full(t_file *list, int type)
{
	int	size[7];
	int	total;
	ft_bzero(size, sizeof(size));
	total = get_block_size(list, size);
	if (type)
		ft_printf("total: %d\n", total);
	while (list)
	{
		print_item(&list, size);
		list = list->next;
	}
}

void	print_list(t_file **list, int flags, int type)
{
	sort_list(list, flags);
	!(LS_L & flags) ? print_simple(*list) : print_full(*list, type);
}

/*
** Initialize folder as list
*/

t_file	*read_folder(char path[PATH_MAX], char *name, int flags)
{
	t_file			*file;
	DIR				*folder;
	struct dirent	*entry;

	file = NULL;
	if (!(folder = opendir(path)))
		error(name, ERRNO);
	else
	{
		while ((entry = readdir(folder)))
			if ((flags & LS_A) || entry->d_name[0] != '.')
				add_new_file(path, entry->d_name, &file);
		closedir(folder);
	}
	return (file);
}

static void		print_path(char *full_path, int count, int *first)
{
	if (count != 0 && count != 1)
	{
		if (*first == 2)
		{
			*first = 1;
			ft_printf("%s:\n", full_path);
		}
		else
			ft_printf("\n%s:\n", full_path);
	}
}

void	free_list(t_file **list)
{
	t_file	*next;

	next = *list;
	while (next)
	{
		next = (*list)->next;
		free((*list)->name);
		free(*list);
		*list = next;
	}
}

void	print_folders(t_file *list, int flags, int first, int count)
{
	t_file	*file;

	if (!(flags & LS_RR) && !first)
		return ;
	while (list)
	{
		if (S_ISDIR(list->mode) && (first || (ft_strcmp(list->name, ".")
			&& ft_strcmp(list->name, ".."))))
		{
			print_path(list->full_path, count, &first);
			file = read_folder(list->full_path, list->name, flags);
			if (file)
			{
				print_list(&file, flags, 1);
				print_folders(file, flags, 0, -1);
				free_list(&file);
			}
		}
		list = list->next;
	}
}

int		print_files(t_file *list, int flags)
{
	int		i;
	t_file	*curr;

	i = 0;
	curr = NULL;
	while (list)
	{
		if (!S_ISDIR(list->mode))
		{
			add_new_file("", list->name, &curr);
			i++;
		}
		list = list->next;
	}
	print_list(&curr, flags, 0);
	free_list(&curr);
	return (i);
}

void	print_args(t_file *list, int flags, int count)
{
	int		i;

	i = print_files(list, flags);
	if (count - i > 0)
		print_folders(list, flags, i ? 1 : 2, count);
}

int		main(int ac, char **av)
{
	int		i;
	int		flags;
	t_file	*list;

	i = parse_options(ac, av, &flags);
	list = init_list(ac - i, av + i);
	print_args(list, flags, !(ac - i) ? 1 : ac - i);
	return (0);
}
