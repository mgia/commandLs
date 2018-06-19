/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtan <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 11:10:16 by mtan              #+#    #+#             */
/*   Updated: 2018/06/19 11:10:21 by mtan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	parse_option(char *s, int *flags)
{
	int		i;

	while (*(++s))
	{
		if ((i = ft_strchri("lRart", *s)) == -1)
			error(s, USAGE);
		*flags |= (1 << i);
	}
}

int		parse_options(int ac, char **av, int *flags)
{
	int		i;

	*flags = 0;
	i = 0;
	while (++i < ac && av[i][0] == '-' && av[i][1])
		parse_option(av[i], flags);
	return (i);
}
