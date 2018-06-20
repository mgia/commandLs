/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:10:16 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:10:21 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_list(t_file **list, int flags, int type)
{
	sort_list(list, flags);
	!(LS_L & flags) ? print_matrix(*list) : print_details(*list, type);
}

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

void	print_folders(t_file *list, int flags, int first, int ac)
{
	t_file	*file;

	if (!(flags & LS_RR) && !first)
		return ;
	while (list)
	{
		if (S_ISDIR(list->mode) && (first || (ft_strcmp(list->name, ".")
			&& ft_strcmp(list->name, ".."))))
		{
			print_path(list->full_path, ac, &first);
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

void	print_args(t_file *list, int flags, int count)
{
	int		i;

	sort_list(&list, flags);
	i = print_files(list, flags);
	if (count - i > 0)
		print_folders(list, flags, i ? 1 : 2, count);
}
