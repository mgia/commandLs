/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:10:16 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:10:21 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_file	*initialize_list(int count, char **names)
{
	t_file	*head;
	int		i;

	head = NULL;
	if (!count)
		add_new_file("", ".", &head);
	else
	{
		i = -1;
		while (++i < count)
			if (add_new_file("", names[i], &head) == -1)
				error(names[i], ERRNO);
	}
	return (head);
}

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

void	print_path(char *full_path, int count, int *first)
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
