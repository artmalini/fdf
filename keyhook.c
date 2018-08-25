/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhook.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 13:35:15 by amakhiny          #+#    #+#             */
/*   Updated: 2018/02/28 13:35:20 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

void	print_help(t_vis *prm)
{
	mlx_string_put(prm->mlx, prm->win, 10, 10, 0xf4d942, "ESC to quit");
	mlx_string_put(prm->mlx, prm->win, 10, 35, 0xf4d942, "Use arrows to move");
	mlx_string_put(prm->mlx, prm->win, 10, 65, 0xf4d942,
		"Mouse whell to resize altitude");
	mlx_string_put(prm->mlx, prm->win, 10, 95, 0xf4d942, "Keys to scale + - ");
}

int		keyboard_vis_hook(int key, t_vis *prm)
{
	if (key == 53)
		exit(EXIT_SUCCESS);
	else if (key == UP)
		prm->pos_y -= 10;
	else if (key == DOWN)
		prm->pos_y += 10;
	else if (key == LEFT)
		prm->pos_x -= 10;
	else if (key == RIGHT)
		prm->pos_x += 10;
	else if (key == PLUS)
	{
		prm->size += 1;
		if (prm->size > 100)
			prm->size = 100;
	}
	else if (key == MINUS)
	{
		prm->size -= 1;
		if (prm->size < 1)
			prm->size = 1;
	}
	image_builder(prm);
	return (0);
}

int		mouse_vis_hook(int key, int x, int y, t_vis *prm)
{
	x = 0;
	y = 0;
	if (key == 4)
	{
		prm->zoom += .1;
		if (prm->zoom == 10)
			prm->zoom = 10;
	}
	else if (key == 5)
	{
		prm->zoom -= .1;
		if (prm->zoom == 0)
			prm->zoom = .1;
	}
	image_builder(prm);
	return (0);
}
