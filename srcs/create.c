/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:10:15 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:10:21 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	get_full_path(char *path, char *name, char full_path[PATH_MAX])
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
		errno = ENAMETOOLONG;
	return (0);
}

t_file		*create_file(char *path, char *name, t_stat *stat)
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

int			add_new_file(char *path, char *name, t_file **lst)
{
	char	full_path[PATH_MAX];
	t_stat	stat;

	if (!(get_full_path(path, name, full_path)))
		return (error(name, 1));
	if (lstat(full_path, &stat) == -1)
		return (-1);
	if (!(*lst))
		(*lst) = create_file(path, name, &stat);
	else
	{
		while ((*lst)->next)
			lst = &((*lst)->next);
		(*lst)->next = create_file(path, name, &stat);
	}
	return (1);
}
