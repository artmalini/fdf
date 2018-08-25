/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildcard.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 13:47:08 by amakhiny          #+#    #+#             */
/*   Updated: 2018/02/28 13:47:13 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

int		build_nbr(t_vis *prm, char **out, int xsize, int i)
{
	int		val;
	int		k;
	int		space;
	char	**tmp;

	space = -1;
	val = 0;
	tmp = ft_strsplit(out[0], ' ');
	k = -1;
	while (++k < xsize)
		prm->card[i][k] = ft_atoi(tmp[k]);
	free_map(tmp, xsize);
	free_map(out, 1);
	return (0);
}

int		build_nbr_hex(t_vis *prm, char **hex, int xsize, int i)
{
	int		k;
	int		space;
	char	**tmp;
	char	*str;

	k = -1;
	tmp = ft_strsplit(hex[0], ' ');
	while (++k < xsize)
	{
		str = tmp[k];
		space = -1;
		while (str[++space])
		{
			if (str[space + 1] == ',')
			{
				prm->hex[i][k] = hex_int(str + 2);
				break ;
			}
			else
				prm->hex[i][k] = ft_atoi(tmp[k]);
		}
	}
	free_map(tmp, xsize);
	free_map(hex, 1);
	return (0);
}

void	build_card_pars(t_vis *prm, int fd, char *output, int i)
{
	char	**tmp;
	char	**hexim;

	while (get_next_line(fd, &output) > 0)
	{
		if (output[0] != '\0')
		{
			prm->xcard = map_check2(output);
			prm->card[i] = (int *)malloc(sizeof(int) * prm->xcard);
			prm->hex[i] = (int *)malloc(sizeof(int) * prm->xcard);
			if (!ft_strchr(output, '\n'))
			{
				tmp = ft_strsplit(output, '\n');
				hexim = ft_strsplit(output, '\n');
			}
			if (tmp[0])
			{
				build_nbr(prm, tmp, prm->xcard, i);
				build_nbr_hex(prm, hexim, prm->xcard, i);
			}
			i++;
		}
		free(output);
	}
	free(output);
}

void	build_card(t_vis *prm, int fd, char *output)
{
	prm->card = (int **)malloc(sizeof(int *) * prm->ycard);
	prm->hex = (int **)malloc(sizeof(int *) * prm->ycard);
	build_card_pars(prm, fd, output, 0);
}
