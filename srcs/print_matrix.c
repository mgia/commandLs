/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_matrix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:33:34 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:33:39 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

void	print_matrix(t_file *list)
{
	t_point		max;
	t_point		dims;

	get_dimensions(list, &max, &dims);
	print_dimensions(list, &max, &dims);
}
