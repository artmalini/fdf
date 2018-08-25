/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vismap.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 13:39:44 by amakhiny          #+#    #+#             */
/*   Updated: 2018/02/28 13:39:47 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

void	fdf_color(t_vis *prm, int x, int y)
{
	int		p;

	p = x * 4 + y * prm->l;
	if (y > 0 && y < H && x > 0 && x < W)
	{
		prm->drw[p] = prm->b;
		prm->drw[p + 1] = prm->g;
		prm->drw[p + 2] = prm->r;
	}
}

void	color_paint(t_place v0, t_place v1, t_vis *prm)
{
	t_place		d;
	t_place		s;
	int			err;
	int			e2;

	d.x = ABS(v1.x - v0.x);
	d.y = ABS(v1.y - v0.y);
	s.x = v0.x >= v1.x ? -1 : 1;
	s.y = v0.y >= v1.y ? -1 : 1;
	err = (d.x > d.y ? d.x : -d.y) / 2;
	while (1)
	{
		fdf_color(prm, v0.x, v0.y);
		if (v0.y == v1.y && v0.x == v1.x)
			break ;
		e2 = err;
		if (e2 > -d.x && ((err -= d.y) || !err))
			v0.x += s.x;
		if (e2 < d.y)
		{
			err += d.x;
			v0.y += s.y;
		}
	}
}

void	border_y(t_vis *prm)
{
	int			y;
	int			x;
	t_place		v0;
	t_place		v1;

	y = -1;
	while (++y < prm->ycard - 1)
	{
		x = prm->xcard - 1;
		v0.y = prm->pos_y + (y * prm->size - (prm->card[y][x] * prm->zoom));
		v0.x = (prm->pos_x - (y * prm->size / 2)) + x * prm->size;
		v1.y = prm->pos_y + ((y + 1) * prm->size - (prm->card[y + 1]
			[x] * prm->zoom));
		v1.x = prm->pos_x - ((y + 1) * prm->size / 2) + x * prm->size;
		color_paint(v0, v1, prm);
	}
}

void	border_x(t_vis *prm)
{
	int			y;
	int			x;
	t_place		v0;
	t_place		v1;

	y = prm->ycard - 1;
	x = -1;
	while (++x < prm->xcard - 1)
	{
		v0.y = prm->pos_y + (y * prm->size - (prm->card[y][x] * prm->zoom));
		v0.x = (prm->pos_x - (y * prm->size / 2)) + x * prm->size;
		v1.y = prm->pos_y + (y * prm->size - (prm->card[y][x + 1] * prm->zoom));
		v1.x = (prm->pos_x - (y * prm->size / 2)) + (x + 1) * prm->size;
		color_paint(v0, v1, prm);
	}
}

void	vis_map(t_vis *p)
{
	int			i;
	int			j;
	t_place		v0;
	t_place		v1;

	i = -1;
	while (++i < p->ycard - 1)
	{
		j = -1;
		while (++j < p->xcard - 1)
		{
			fit_color(p, j, i);
			v0.y = p->pos_y + (i * p->size - (p->card[i][j] * p->zoom));
			v0.x = p->pos_x - (i * p->size / 2) + j * p->size;
			v1.y = p->pos_y + ((i + 1) * p->size - (p->card[i + 1]
													[j] * p->zoom));
			v1.x = p->pos_x - ((i + 1) * p->size / 2) + j * p->size;
			color_paint(v0, v1, p);
			v1.y = p->pos_y + (i * p->size - (p->card[i][j + 1] * p->zoom));
			v1.x = p->pos_x - (i * p->size / 2) + (j + 1) * p->size;
			color_paint(v0, v1, p);
		}
		border_y(p);
		border_x(p);
	}
}
