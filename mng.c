/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mng.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 13:50:03 by amakhiny          #+#    #+#             */
/*   Updated: 2018/02/28 13:50:08 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

int		print_err(char *err)
{
	ft_putstr(err);
	return (0);
}

void	free_map(char **map, int size)
{
	int		i;

	i = -1;
	while (++i < size)
		ft_memdel((void **)&map[i]);
	ft_memdel((void **)&map);
}

void	free_int_map(t_vis *prm, int size)
{
	int		i;

	i = -1;
	while (++i < size)
		ft_memdel((void **)&prm->card[i]);
	ft_memdel((void **)&prm->card);
}
