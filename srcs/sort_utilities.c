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

t_file		*join(t_file *a, t_file *b, int (*f)(t_file*, t_file*))
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

static void	split_list(t_file *source, t_file **front_ref, t_file **back_ref)
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
	*front_ref = source;
	*back_ref = slow->next;
	slow->next = NULL;
}

void		merge_sort(t_file **head_ref, int (*f)(t_file*, t_file*))
{
	t_file		*head;
	t_file		*a;
	t_file		*b;

	head = *head_ref;
	if (!head || !head->next)
		return ;
	split_list(head, &a, &b);
	merge_sort(&a, f);
	merge_sort(&b, f);
	*head_ref = join(a, b, f);
}

void		sort_list(t_file **list, int flags)
{
	merge_sort(list, &ft_ascii);
	if (flags & LS_T)
		merge_sort(list, &ft_time);
	if (flags & LS_R)
		reverse_list(list);
}
