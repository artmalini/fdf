/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/24 11:09:50 by amakhiny          #+#    #+#             */
/*   Updated: 2018/02/24 11:11:45 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

# include <mlx.h>
# include "libft/libft.h"
# include "includes/get_next_line.h"

#include <stdio.h>
#include <math.h>
/*# define ESC			53
# define LEFT			123
# define RIGHT			124
# define UP				126
# define DOWN			125
# define MINUS			78
# define PLUS			69
# define KEY_C			8
# define STAR			67
# define SLASH			75

# define BLUE			0x0000FF
# define GREEN			0x00F611
# define BROWN			0x541919
# define WHITE			0xFFFFFF

# define WIN_X			1000
# define WIN_Y			1080
*/

# define H			1000
# define W			1080
# define UP			126
# define DOWN		125
# define RIGHT		124
# define LEFT		123
# define PLUS		69
# define MINUS		78
# define BLUE			0x0000FF
# define GREEN			0x00F611
# define BROWN			0x541919
# define WHITE			0xFFFFFF
# define ABS(x)		((x) < 0 ? -(x) : (x))

typedef struct			s_point
{
	int					x;
	int					y;
}						t_place;

typedef struct 	s_viz
{
	int 		**card;
	int 		ycard;
	int 		xcard;
	int			size;
	int			size_line;
	int			pos_x;
	int			pos_y;	
	double			zoom;
	int			angl_x;
	int			angl_y;
	int			angl_z;

	int 		r;
	int 		g;
	int 		b;
	int 		stop;
	int 		err;
	int 		ymax;
	int 		xmax;

	char 		*drw;
	int 		e;
	int			bi;
	int 		l;
	void 		*mlx;
	void 		*win;
	void		*img;
}				t_vis;

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

int		map_check_char(char *out)
{
	int		i;
	int		space;

	space = 0;
	i = -1;
	while (out[++i] && out[0] != '\0')
	{
		if (((out[i] >= 48 && out[i] <= 57) || (out[i] >= 65 && out[i]
			<= 70) || (out[i] >= 97 && out[i] <= 102)) ||
			(out[i] == ' ' || out[i] == '\n'))
			space++;
	}
	if (space != i)
		return (0);
	//fprintf(stderr, "space %d\n", space);	
	return (1);
}

int		mapsize_and_check2(char *out)
{
	int		i;
	int		space;

	space = 0;
	i = -1;
	while (out[++i] && out[0] != '\0')
	{
		if (((out[i] >= 48 && out[i] <= 57) || (out[i] >= 65 && out[i]
			<= 70) || (out[i] >= 97 && out[i] <= 102)) &&
			(out[i + 1] == ' ' || out[i + 1] == '\0'))
			space++;
	}
	//fprintf(stderr, "space %d\n", space);	
	return (space);
}

int		get_max_x(t_vis *prm, char *output)
{
	int		val;

	val = mapsize_and_check2(output);
	//prm->xcard = val;
	if (val > prm->xcard)
		return (val);
	else
		return (prm->xcard);
}

int		mapsize_and_check(int fd)
{	
	int		val;
	int		space;
	char	*output;
	
	val = 0;
	space = 0;
	while (get_next_line(fd, &output) > 0)
	{		
		space = mapsize_and_check2(output);
		if (!map_check_char(output))
			space = 0;
		if (space == 0)
		{
			free(output);
			return (0);
		}
		//prm->xcard = get_max_x(prm, output);
		//fprintf(stderr, "prm->xcard %d\n", prm->xcard);
		space = 0;
		val++;
		free(output);
	}
	free(output);
	return (val);
}

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
	{
		//prm->card[j][k] = ft_atoi(tmp[k]);
		prm->card[i][k] = ft_atoi(tmp[k]);
		//fprintf(stderr, "tmp[k] %s prm->card[i][k] %d", tmp[k], prm->card[i][k]);
		//fprintf(stderr, "%d", prm->card[i][k]);
	}
	free_map(tmp, xsize);
	free_map(out, 1);
	return (0);
}



void	build_card(t_vis *prm, int fd, char *output, char **arg)
{
	int		i;
	int		j;
	int		k;
	char	**tmp;

	i = 0;
	j = 0;
	fd = open(arg[1], O_RDONLY);
	prm->card = (int **)malloc(sizeof(int *) * prm->ycard);
	while (get_next_line(fd, &output) > 0)
	{		
		if (output[0] != '\0')
		{
			//prm->xcard = get_max_x(prm, output);
			prm->xcard = mapsize_and_check2(output);
			prm->card[i] = (int *)malloc(sizeof(int) * prm->xcard);
			if (!ft_strchr(output, '\n'))
				tmp = ft_strsplit(output, '\n');

			//fprintf(stderr, "strlen %d tmp[0] %s\n", prm->xcard, tmp[0]);
			if (tmp[j])
			{
				build_nbr(prm, tmp, prm->xcard, i);
				fprintf(stderr, "\n");
			}
			j = 0;
			i++;
		}
		free(output);
	}
	free(output);
}





void		fit_color(t_vis *env, int x, int y)
{
	//fprintf(stderr, "prm->card[y][x] * prm->zoom %f\n", env->card[y][x] * env->zoom);	
	double	val;
	intmax_t	i;
	//int			hex;

	//hex = 0x7f8711;
	val = env->card[y][x] * env->zoom;
	i = 1;
	i *= val; 
	//(val == 0) ? val = 0 : val;
	//fprintf(stderr, "### %f\n", ((hex >> 16) & 0xFF) / 255.0);
	//fprintf(stderr, "### %f\n", ((hex >> 8) & 0xFF) / 255.0);
	//fprintf(stderr, "e->card[y][x] * e->zoom %ju\n", i);
	if (i == 0)		
	{
		env->r = 55;
		env->g = 244;
		env->b = 89;
		// env->r = ((hex >> 16) & 0xFF) / 255.0;
		// env->g = ((hex >> 8) & 0xFF) / 255.0;
		// env->b = ((hex) & 0xFF) / 255.0;
	}
	else if (i < 0)
	{
		env->r = 66;
		env->g = 128;
		env->b = 244;
	}
	else if (i > 0 && i < 80)
	{
		env->r = 96;
		env->g = 66;
		env->b = 49;
	}
	else
	{
		env->r = 255;
		env->g = 255;
		env->b = 255;
	}
	// if (env->card[y][x] <= 0)
	// 	return (55);
	// else if (env->card[y][x] > 0 && env->card[y][x] <= 40)
	// 	return (100);
	// else if (env->card[y][x] > 40 && env->card[y][x] <= 120)
	// 	return (180);
	// else
	// 	return (255);
}




void	draw_color_in_image(t_vis *prm, int x, int y)
{
	//t_color	u;
	int		p;

	//u.color = prm->color;
	p = x * 4 + y * prm->size_line;
	//fprintf(stderr, "p %d\n", p);
	if (y > 0 && y < H && x > 0 && x < W)
	{
		// prm->img[p] = u.rgb.b;
		// prm->img[p + 1] = u.rgb.g;
		// prm->img[p + 2] = u.rgb.r;
		//prm->drw[p] = 89;
		//prm->drw[p + 1] = 244;
		//prm->drw[p + 2] = 66;	
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
	s.x = v0.x < v1.x ? 1 : -1;
	s.y = v0.y < v1.y ? 1 : -1;
	err = (d.x > d.y ? d.x : -d.y) / 2;
	//fprintf(stderr, "err %d\n", err);
	while (1)
	{
		//fprintf(stderr, " v0.x %d v0.y %d\n", v0.x, v0.y);
		draw_color_in_image(prm, v0.x, v0.y);
		if (v0.x == v1.x && v0.y == v1.y)
			break ;
		e2 = err;
		//fprintf(stderr, "e2 %d\n", e2);
		if (e2 > -d.x && ((err -= d.y) || !err))
			v0.x += s.x;
		if (e2 < d.y)
		{
			//fprintf(stderr, "######### (e2 < d.y )e2 %d d.y %d\n", e2, d.y);
			err += d.x;
			v0.y += s.y;
		}
	}
}


void	border_y(t_vis *prm)
{
	int		y;
	int		x;
	t_place	v0;
	t_place	v1;
	//int		xx;

	y = 0;
	while (y < prm->ycard - 1)
	{
		x = prm->xcard - 1;
		//xx = prm->pos_x - (y * prm->size / 2);
		//get_color(y, x, e);
		v0.y = prm->pos_y + (y * prm->size - (prm->card[y][x] * prm->zoom));
		v0.x = (prm->pos_x - (y * prm->size / 2)) + x * prm->size;
		v1.y = prm->pos_y + ((y + 1) * prm->size - (prm->card[y + 1][x] * prm->zoom));
		v1.x = prm->pos_x - ((y + 1) * prm->size / 2) + x * prm->size;
		color_paint(v0, v1, prm);
		y++;
	}
}

void	border_x(t_vis *prm)
{
	int		y;
	int		x;
	t_place	v0;
	t_place v1;
	//int		xx;

	y = prm->ycard - 1;
	x = 0;
	//xx = prm->pos_x - (y * prm->size / 2);
	while (x < prm->xcard - 1)
	{
		//get_color(y, x, e);
		v0.y = prm->pos_y + (y * prm->size - (prm->card[y][x] * prm->zoom));
		v0.x = (prm->pos_x - (y * prm->size / 2)) + x * prm->size;
		v1.y = prm->pos_y + (y * prm->size - (prm->card[y][x + 1] * prm->zoom));
		v1.x = (prm->pos_x - (y * prm->size / 2)) + (x + 1) * prm->size;
		color_paint(v0, v1, prm);
		x++;
	}
}

void		color_paint_border(t_vis *prm)
{
	border_y(prm);
	border_x(prm);
}

void	vis_map(t_vis *prm)
{
	int		i;
	int		j;
	t_place	v0;
	t_place	v1;
	//int		jj;

	float		angle;

	i = -1;
	//printf("###OK\n");
	//fprintf(stderr, "prm->icard %d prm->jcard %d\n", prm->icard, prm->jcard);
	while (++i < prm->ycard - 1)
	{
		j = -1;
		//jj = prm->pos_x - (i * prm->size / 2);
		while (++j < prm->xcard - 1)
		{
			fit_color(prm, j, i);
			angle = (3.14159265 * prm->angl_x) / 180;
			v0.y = prm->pos_y + ((i * prm->size) - (prm->card[i][j] * prm->zoom));
			v0.x = (prm->pos_x - (i * prm->size / 2)) + j * prm->size;

			v0.y = v0.y * cos(angle) + v0.x * sin(angle);
			v0.x = v0.x * cos(angle) - v0.y * sin(angle);
			v0.y = round(v0.y);
			v0.x = round(v0.x);
			//v0.y = prm->pos_y + (i * prm->size - (prm->card[i][j] * prm->zoom));
			//v0.x = (prm->pos_x - (i * prm->size / 2)) + j * prm->size;
				//fprintf(stderr, "v0.y %d v0.x %d\n", v0.y, v0.x);
			angle = (3.14159265 * prm->angl_y) / 180;
			v1.y = prm->pos_y + ((i + 1) * prm->size - (prm->card[i + 1][j] * prm->zoom));
			v1.x = prm->pos_x - ((i + 1) * prm->size / 2) + j * prm->size;
			v1.y = v1.y * cos(angle) - v1.x * sin(angle);
			v1.x = v1.x * cos(angle) + v1.y * sin(angle);
			v1.y = round(v1.y);
			v1.x = round(v1.x);
				//fprintf(stderr, "(i + 1) v0.y %d v0.x %d\n", v0.y, v0.x);
			color_paint(v0, v1, prm);

			angle = (3.14159265 * prm->angl_z) / 180;
			v1.y = prm->pos_y + (i * prm->size - (prm->card[i][j + 1] * prm->zoom));
			v1.x = (prm->pos_x - (i * prm->size / 2)) + (j + 1) * prm->size;
				//fprintf(stderr, " (j + 1) v0.y %d v0.x %d\n", v0.i, v0.j);
			v1.y = v1.y * cos(angle) + v1.x * sin(angle);
			v1.x = v1.x * cos(angle) - v1.y * sin(angle);
			v1.y = round(v1.y);
			v1.x = round(v1.x);			
			color_paint(v0, v1, prm);
		}
		//fprintf(stderr, "\n");
		color_paint_border(prm);
	}
}


int		image_builder(t_vis *prm)
{
	//if (prm->img == NULL)
	//	create_image(prm);
	//ft_draw_image(prm);
	prm->drw = mlx_get_data_addr(prm->img, &(prm->bi), &(prm->size_line), &(prm->e));
	vis_map(prm);
	mlx_put_image_to_window(prm->mlx, prm->win, prm->img, 0, 0);
	mlx_destroy_image(prm->mlx, prm->img);
	prm->img = mlx_new_image(prm->mlx, W, H);
	//mlx_string_put(prm->mlx, prm->win, (W / 2), (H / 2), 0x0FFFFFF, ft_itoa(prm->xcard));
	// if (prm->ui == 1)
	// {
	// 	display_key(prm);
	// 	ft_display_variable(prm);
	// }	
	return (0);
}


int		keyboard_vis_hook(int key, t_vis *prm)
{
	fprintf(stderr, "key %d\n", key);
	if (key == 53)
	{
		//system("leaks fdf");
		exit(1);
	}
	else if (key == UP)
		prm->pos_y -= 5;
	else if (key == DOWN)
		prm->pos_y += 5;
	else if (key == LEFT)
		prm->pos_x -= 5;
	else if (key == RIGHT)
		prm->pos_x += 5;
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
	else if (key == 13)
			prm->angl_y += 1;
	else if (key == 1)
			prm->angl_y -= 1;
	else if (key == 0)
			prm->angl_x += 1;
	else if (key == 2)
			prm->angl_x -= 1;
	image_builder(prm);
	return (0);
}

int		mouse_vis_hook(int key, int x, int y, t_vis *prm)
{
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

int		build_vis(t_vis *prm)
{
	prm->mlx = mlx_init();
	prm->win = mlx_new_window(prm->mlx, W, H, "FDF");
	prm->img = mlx_new_image(prm->mlx, W, H);
	mlx_hook(prm->win, 2, 1, keyboard_vis_hook, prm);
	mlx_mouse_hook(prm->win, mouse_vis_hook, prm);
	image_builder(prm);
	//mlx_loop_hook(prm->mlx, image_builder, prm);

	mlx_loop(prm->mlx);	
	//free all vis map!!!!! ++++++++++++++++++++++++++++++++++
	return (0);		
}

void	struct_init(t_vis *prm)
{
	prm->img = NULL;
	prm->xcard = 0;
	prm->ycard = 0;
	prm->size_line = W;
	prm->size = 8;
	prm->pos_x = 500;
	prm->pos_y = 500;
	prm->zoom = 2;
	prm->angl_x = 0;
	prm->angl_y = 0;
	prm->angl_z = 0;
}

void	parse_file(t_vis *prm, char **arg)
{
	int		fd;
	
	
	//int		ysize;
	//int		xsize;
	char	*output;

	
	prm->ycard = 0;
	if(!(fd = open(arg[1], O_RDONLY)))
		exit(1);
	if(!(prm->ycard = mapsize_and_check(fd)))
	{
		print_err("Wrong map\n");
		exit(1);
	}
	close(fd);
	build_card(prm, fd, output, arg);
	//free(output);
	close(fd);
	build_vis(prm);

	//free_int_map(prm, prm->ycard);
	//printf("output %p\n", output);
}

int		main(int argc, char **argv)
{
	t_vis	*prm;

	if (argc != 2)
		return(print_err("Wrong input\n"));
	prm = (t_vis *)malloc(sizeof(t_vis));
	struct_init(prm);
	parse_file(prm, argv);

	free_int_map(prm, prm->ycard);
	free(prm);

	//system("leaks fdf");
	return (0);
}



