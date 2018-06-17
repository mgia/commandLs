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

# define ASCII 0
# define REVERSE 1
# define TIME 2

// static int	sort_validate(t_file *a, t_file *b, int option)
// {
// 	if (option == REVERSE)
// 		return ((ft_strcmp(a->name, b->name) > 0));
// 	else (option == TIME
// 			? (
// 				a->time > b->time ||
// 				(a->time == b->time && a->ntime > b->ntime)
// 			)
// 			: (ft_strcmp(a->name, b->name) < 0)
// 		)
// 	)
// }

t_file	*SortedMerge(t_file *a, t_file *b, int option)
{
	t_file	*result;

	result = NULL;
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);
	if (option == REVERSE
		? (ft_strcmp(a->name, b->name) > 0)
		: (option == TIME
			? (
				a->time > b->time ||
				(a->time == b->time && a->ntime > b->ntime)
			)
			: (ft_strcmp(a->name, b->name) < 0)
		)
	)
	{
		result = a;
		result->next = SortedMerge(a->next, b, option);
	}
	else
	{
		result = b;
		result->next = SortedMerge(a, b->next, option);
	}
	return (result);
}

void FrontBackSplit(t_file *source,
	t_file** frontRef, t_file** backRef)
	{
		t_file* fast;
		t_file* slow;
		slow = source;
		fast = source->next;

		/* Advance 'fast' two nodes, and advance 'slow' one node */
		while (fast != NULL)
		{
			fast = fast->next;
			if (fast != NULL)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}

		/* 'slow' is before the midpoint in the list, so split it in two
		at that point. */
		*frontRef = source;
		*backRef = slow->next;
		slow->next = NULL;
}

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(t_file **headRef, int option)
{
	t_file*		head = *headRef;
	t_file*		a;
	t_file*		b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->next == NULL))
		return;

	/* Split head into 'a' and 'b' sublists */
	FrontBackSplit(head, &a, &b);

	/* Recursively sort the sublists */
	MergeSort(&a, option);
	MergeSort(&b, option);

	/* answer = merge the two sorted lists together */
	*headRef = SortedMerge(a, b, option);
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
	MergeSort(list, ASCII);
	if (flags & LS_T)
		MergeSort(list, TIME);
	if (flags & LS_R)
		reverse_list(list);
}

/*
** Print functions
*/

void	print_list(t_file *list, int flags)
{
	sort_list(&list, flags);
	flags += 0;
	while (list)
	{
		ft_printf("%s ", (list)->name);
		(list) = (list)->next;
	}
}

static void		display_full_path(char *full_path, int count, int *first)
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

void	print_folders(t_file *lst, int flags, int first, int count)
{
	t_file	*file;

	if (!(flags & LS_RR) && !first)
		return ;
	while (lst)
	{
		if (S_ISDIR(lst->mode))
		{
			display_full_path(lst->full_path, count, &first);
			file = read_folder(lst->full_path, lst->name, flags);
			if (file)
			{
				print_list(file, flags);
				print_folders(file, flags, 0, -1);
			}
		}
		lst = lst->next;
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
			i = 0;
		}
		list = list->next;
	}
	print_list(curr, flags);
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
