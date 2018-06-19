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

int		ft_ascii(t_file *a, t_file *b)
{
	return (ft_strcmp(a->name, b->name) < 0);
}

int		ft_time(t_file *a, t_file *b)
{
	return (a->time > b->time || (a->time == b->time
		&& a->ntime > b->ntime));
}
