/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_details.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:33:34 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:33:39 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

void	get_block_size(t_file *list, int size[7], int *total)
{
	*total = 0;
	size[0] = 0;
	while (list)
	{
		size[1] = ft_max(ft_nbrlen(list->st_nlink), size[1]);
		size[2] = ft_max(ft_strlen(getpwuid(list->st_uid)->pw_name), size[2]);
		size[3] = ft_max(ft_strlen(getgrgid(list->st_gid)->gr_name), size[3]);
		size[4] = ft_max(ft_nbrlen(list->size), size[4]);
		*total += list->st_blocks;
		list = list->next;
	}
}

void	print_details(t_file *list, int type)
{
	int	size[7];
	int	total;

	ft_bzero(size, sizeof(size));
	get_block_size(list, size, &total);
	if (type)
		ft_printf("total: %d\n", total);
	while (list)
	{
		print_item(&list, size);
		list = list->next;
	}
}
