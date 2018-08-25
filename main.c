/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 18:51:02 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/13 18:51:06 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

int		image_builder(t_vis *prm)
{
	prm->drw = mlx_get_data_addr(prm->img, &(prm->bi), &(prm->l), &(prm->e));
	vis_map(prm);
	mlx_put_image_to_window(prm->mlx, prm->win, prm->img, 0, 0);
	mlx_destroy_image(prm->mlx, prm->img);
	prm->img = mlx_new_image(prm->mlx, W, H);
	print_help(prm);
	return (0);
}

int		build_vis(t_vis *prm)
{
	prm->mlx = mlx_init();
	prm->win = mlx_new_window(prm->mlx, W, H, "FDF");
	prm->img = mlx_new_image(prm->mlx, W, H);
	mlx_hook(prm->win, 2, 1, keyboard_vis_hook, prm);
	mlx_mouse_hook(prm->win, mouse_vis_hook, prm);
	image_builder(prm);
	mlx_loop(prm->mlx);
	return (0);
}

void	struct_init(t_vis *prm)
{
	prm->card = NULL;
	prm->hex = NULL;
	prm->img = NULL;
	prm->xcard = 0;
	prm->ycard = 0;
	prm->l = W;
	prm->size = 8;
	prm->pos_x = W / 4;
	prm->pos_y = H / 2;
	prm->zoom = 2;
	prm->r = 0;
	prm->g = 0;
	prm->b = 0;
}

void	parse_file(t_vis *prm, char **arg)
{
	int		fd;
	char	*output;

	prm->ycard = 0;
	output = NULL;
	if (!(fd = open(arg[1], O_RDONLY)))
		exit(1);
	if (!(prm->ycard = map_check(fd)))
	{
		print_err("Wrong map\n");
		exit(1);
	}
	close(fd);
	fd = open(arg[1], O_RDONLY);
	build_card(prm, fd, output);
	close(fd);
	build_vis(prm);
}

int		main(int argc, char **argv)
{
	t_vis	*prm;

	if (argc != 2)
		return (print_err("Wrong input argument\n"));
	if (!(prm = (t_vis *)malloc(sizeof(t_vis))))
		print_err("Memmory error\n");
	struct_init(prm);
	parse_file(prm, argv);
	free_int_map(prm, prm->ycard);
	free(prm);
	return (0);
}
