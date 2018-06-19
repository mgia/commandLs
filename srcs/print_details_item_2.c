/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_details_item_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:33:34 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:33:39 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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
