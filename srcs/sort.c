/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:10:16 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:10:21 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int		ft_ascii(t_file *a, t_file *b)
{
	return (ft_strcmp(a->name, b->name) > 0);
}

static int		ft_time(t_file *a, t_file *b)
{
	return (a->time < b->time || (a->time == b->time
		&& a->ntime < b->ntime));
}

static t_file	*lst_swap(t_file *p1, t_file *p2)
{
	p1->next = p2->next;
	p2->next = p1;
	return (p2);
}

t_file			*lst_sort(t_file *lst, int (*f)(t_file*, t_file*))
{
	if (!lst)
		return (NULL);
	if (lst->next && f(lst, lst->next))
		lst = lst_swap(lst, lst->next);
	lst->next = lst_sort(lst->next, f);
	if (lst->next && f(lst, lst->next))
	{
		lst = lst_swap(lst, lst->next);
		lst->next = lst_sort(lst->next, f);
	}
	return (lst);
}

void		sort_list(t_file **list, int flags)
{
	*list = lst_sort(*list, &ft_ascii);
	if (flags & LS_T)
		*list = lst_sort(*list, &ft_time);
	if (flags & LS_R)
		reverse_list(list);
}
