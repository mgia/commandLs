/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_details_item_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:33:34 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:33:39 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static char	get_file_type(int mode)
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

void		print_chmod(t_file **file)
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

void		print_links(t_file **file, int size)
{
	ft_putnchar(' ', ft_max(size - ft_nbrlen((*file)->st_nlink), 0) + 2);
	ft_printf("%hu", (*file)->st_nlink);
}

void		print_id(t_file **file, int uid, int gid)
{
	uid = ft_max(uid - ft_strlen(getpwuid((*file)->st_uid)->pw_name), 0) + 2;
	gid = ft_max(gid - ft_strlen(getgrgid((*file)->st_gid)->gr_name), 0);
	ft_putchar(' ');
	ft_printf("%s", getpwuid((*file)->st_uid)->pw_name);
	ft_putnchar(' ', uid);
	ft_printf("%s", getgrgid((*file)->st_gid)->gr_name);
	ft_putnchar(' ', gid);
}
