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

# define ABS(x)			((x) < 0 ? -(x) : (x))

# define ENDIAN			&(e->endian)
# define BBP			&(e->bbp)
# define PIKE			e->spike

typedef unsigned char	t_bytes;

typedef struct			s_rgb
{
	t_bytes				b;
	t_bytes				g;
	t_bytes				r;
}						t_rgb;

typedef union			u_color
{
	size_t				color;
	t_rgb				rgb;
}						t_color;

typedef struct			s_point
{
	int					x;
	int					y;
}						t_point;

typedef struct			s_env
{
	void				*mlx;
	void				*win;
	void				*image;
	char				*image_addr;
	int					id;
	int					size_line;
	int					bbp;
	int					endian;
	size_t				color;
	char				**tab;
	int					**map;
	t_point				**draw;
	int					x;
	int					y;
	int					pos_x;
	int					pos_y;
	double				spike;
	int					pitch;
	int					size;
	int					ui;
}						t_env;

void					ft_draw_border(t_env *env);
int						check_for_char(t_env *env);
void					ft_draw_image(t_env *env);
void					get_color(int y, int x, t_env *env);
void					ft_draw_border(t_env *env);
void					fill_image(t_env *env);
void					create_image(t_env *env);
void					read_that_file(char *filename, t_env *env);
int						**put_in_tab(char *str);
int						ft_key_hook(int keycode, t_env *env);
int						init_struct(t_env *env);
void					ft_draw_line(t_point p1, t_point p2, t_env *e);
int						expose_hook(t_env *env);
void					display_key(t_env *env);
void					ft_display_variable(t_env *env);
void					check_argv(char *arg, t_env *env);


int		ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\t'
			|| c == '\r' || c == '\f')
		return (1);
	else
		return (0);
}

int		ft_ishexa(char c)
{
	if (ft_isdigit(c) || (c >= 'A' && c <= 'F') || c == 'x' || c == ',')
		return (1);
	else
		return (0);
}

int		check_for_char(t_env *env)
{
	int i;
	int j;

	i = 0;
	while (env->tab[i])
	{
		j = 0;
		while (env->tab[i][j])
		{
			if (!ft_ishexa(env->tab[i][j])
			&& !ft_isspace(env->tab[i][j]) && env->tab[i][j] != '-')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void	check_argv(char *arg, t_env *env)
{
	if (!ft_strcmp(arg, "mars"))
		env->id = 2;
	else
		env->id = 1;
}






void	get_color1(int y, int x, t_env *env)
{
	if (env->map[y][x] <= 0)
		env->color = BLUE;
	else if (env->map[y][x] > 0 && env->map[y][x] <= 40)
		env->color = GREEN;
	else if (env->map[y][x] > 40 && env->map[y][x] <= 120)
		env->color = BROWN;
	else
		env->color = WHITE;
}

void	get_color2(int y, int x, t_env *env)
{
	t_color u;

	u.rgb.r = (env->map[y][x] * 10);
	if (u.rgb.r <= 60)
		u.rgb.r = 60;
	if (u.rgb.r > 255)
		u.rgb.r = 255;
	u.rgb.b = 0;
	u.rgb.g = 0;
	env->color = u.color;
}

void	get_color(int y, int x, t_env *env)
{
	if (env->id == 1)
		get_color1(y, x, env);
	else if (env->id == 2)
		get_color2(y, x, env);
	else
		get_color1(y, x, env);
}



static void	ft_draw_y_border(t_env *e)
{
	int		y;
	int		x;
	t_point	p1;
	t_point	p2;
	int		xx;

	y = 0;
	while (y < e->y - 1)
	{
		x = e->x - 1;
		xx = e->pos_x - (y * e->size / 2);
		get_color(y, x, e);
		p1.y = e->pos_y + (y * e->size - (e->map[y][x] * e->spike));
		p1.x = xx + x * e->size;
		p2.y = e->pos_y + ((y + 1) * e->size - (e->map[y + 1][x] * e->spike));
		p2.x = e->pos_x - ((y + 1) * e->size / 2) + x * e->size;
		ft_draw_line(p1, p2, e);
		y++;
	}
}

static void	ft_draw_x_border(t_env *e)
{
	int		y;
	int		x;
	t_point	p1;
	t_point p2;
	int		xx;

	y = e->y - 1;
	x = 0;
	xx = e->pos_x - (y * e->size / 2);
	while (x < e->x - 1)
	{
		get_color(y, x, e);
		p1.y = e->pos_y + (y * e->size - (e->map[y][x] * e->spike));
		p1.x = xx + x * e->size;
		p2.y = e->pos_y + (y * e->size - (e->map[y][x + 1] * e->spike));
		p2.x = xx + (x + 1) * e->size;
		ft_draw_line(p1, p2, e);
		x++;
	}
}

void		ft_draw_border(t_env *env)
{
	ft_draw_y_border(env);
	ft_draw_x_border(env);
}




void	draw_color_in_image(t_env *env, int x, int y)
{
	t_color	u;
	int		p;

	u.color = env->color;
	p = x * 4 + y * env->size_line;
	if (y > 0 && y < WIN_Y && x > 0 && x < WIN_X)
	{
		env->image_addr[p] = u.rgb.b;
		env->image_addr[p + 1] = u.rgb.g;
		env->image_addr[p + 2] = u.rgb.r;
	}
}

void	create_image(t_env *e)
{
	e->endian = 0;
	e->bbp = 8;
	e->size_line = WIN_X;
	e->image = mlx_new_image(e->mlx, WIN_X, WIN_Y);
	e->image_addr = mlx_get_data_addr(e->image, BBP, &(e->size_line), ENDIAN);
}

void	ft_draw_line(t_point p1, t_point p2, t_env *e)
{
	t_point		d;
	t_point		s;
	int			err;
	int			e2;

	d.x = ABS(p2.x - p1.x);
	d.y = ABS(p2.y - p1.y);
	s.x = p1.x < p2.x ? 1 : -1;
	s.y = p1.y < p2.y ? 1 : -1;
	err = (d.x > d.y ? d.x : -d.y) / 2;
	while (42)
	{
		draw_color_in_image(e, p1.x, p1.y);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		e2 = err;
		if (e2 > -d.x && ((err -= d.y) || !err))
			p1.x += s.x;
		if (e2 < d.y)
		{
			err += d.x;
			p1.y += s.y;
		}
	}
}

void	ft_draw_image(t_env *e)
{
	int		y;
	int		x;
	t_point	p1;
	t_point	p2;
	int		xx;

	y = -1;
	while (++y < e->y - 1)
	{
		x = -1;
		xx = e->pos_x - (y * e->size / 2);
		while (++x < e->x - 1)
		{
			get_color(y, x, e);
			p1.y = e->pos_y + (y * e->size - (e->map[y][x] * PIKE));
			p1.x = xx + x * e->size;
			p2.y = e->pos_y + ((y + 1) * e->size - (e->map[y + 1][x] * PIKE));
			p2.x = e->pos_x - ((y + 1) * e->size / 2) + x * e->size;
			ft_draw_line(p1, p2, e);
			p2.y = e->pos_y + (y * e->size - (e->map[y][x + 1] * PIKE));
			p2.x = xx + (x + 1) * e->size;
			ft_draw_line(p1, p2, e);
		}
	}
	ft_draw_border(e);
}











void	display_key(t_env *env)
{
	mlx_string_put(env->mlx, env->win, 0, 0, WHITE, "ESC to quit");
	mlx_string_put(env->mlx, env->win, 0, 30, WHITE, "Use arrows to move");
	mlx_string_put(env->mlx, env->win, 0, 60, WHITE, "+ & - to resize");
	mlx_string_put(env->mlx, env->win, 0, 90, WHITE, "* & / to resize pikes");
	mlx_string_put(env->mlx, env->win, WIN_X - 93, 0, WHITE, "C to hide");
}

void	ft_display_variable(t_env *env)
{
	mlx_string_put(env->mlx, env->win, 0, 150, WHITE, "Size: ");
	mlx_string_put(env->mlx, env->win, 70, 150, WHITE, ft_itoa(env->size));
	mlx_string_put(env->mlx, env->win, 0, 180, WHITE, "X0: ");
	mlx_string_put(env->mlx, env->win, 70, 180, WHITE, ft_itoa(env->pos_x));
	mlx_string_put(env->mlx, env->win, 0, 210, WHITE, "Y0: ");
	mlx_string_put(env->mlx, env->win, 70, 210, WHITE, ft_itoa(env->pos_y));
}

int		exit_program(int keycode, void *param)
{
	if (keycode == ESC)
	{
		//system("leaks fdf");
		(void)param;
		exit(0);
		return (1);
	}
	return (0);
}

int		handle_size(int keycode, t_env *env)
{
	if (keycode == MINUS)
	{
		env->size = env->size / 2;
		env->pos_x = env->pos_x - 25;
		if (env->size < 1)
			env->size = 1;
		expose_hook(env);
	}
	else if (keycode == PLUS)
	{
		env->size = env->size * 2;
		env->pos_x = env->pos_x + 25;
		if (env->size > 100)
			env->size = 100;
		expose_hook(env);
	}
	return (0);
}

int		handle_pos(int keycode, t_env *env)
{
	if (keycode == LEFT)
	{
		env->pos_x = env->pos_x - 10;
		expose_hook(env);
	}
	else if (keycode == RIGHT)
	{
		env->pos_x = env->pos_x + 10;
		expose_hook(env);
	}
	else if (keycode == UP)
	{
		env->pos_y = env->pos_y - 10;
		expose_hook(env);
	}
	else if (keycode == DOWN)
	{
		env->pos_y = env->pos_y + 10;
		expose_hook(env);
	}
	return (0);
}

int		handle_spike(int keycode, t_env *env)
{
	if (keycode == STAR)
	{
		env->spike = env->spike + 0.1;
		if (env->spike == 10)
			env->spike = 10;
		expose_hook(env);
	}
	else if (keycode == SLASH)
	{
		env->spike = env->spike - 0.1;
		if (env->spike == 0)
			env->spike = 0.1;
		expose_hook(env);
	}
	return (0);
}

int		ft_key_hook(int keycode, t_env *env)
{
	if (keycode == ESC)
		exit_program(keycode, 0);
	else if (keycode == MINUS || keycode == PLUS)
		handle_size(keycode, env);
	else if (keycode == LEFT || keycode == RIGHT
			|| keycode == UP || keycode == DOWN)
		handle_pos(keycode, env);
	else if (keycode == SLASH || keycode == STAR)
		handle_spike(keycode, env);
	else if (keycode == KEY_C)
	{
		env->ui = !env->ui ? 1 : 0;
		expose_hook(env);
	}
	return (0);
}




int		init_struct(t_env *env)
{
	if (!(env->mlx = mlx_init()))
		return (0);
	if (!(env->win = mlx_new_window(env->mlx, WIN_X, WIN_Y, "FdF")))
		return (0);
	env->id = 1;
	env->size = 4;
	env->pitch = 4;
	env->spike = 1;
	env->pos_x = 250;
	env->pos_y = 150;
	env->ui = 1;
	env->image = NULL;
	env->image_addr = NULL;
	return (1);
}

void	ft_clean(t_env *env)
{
	t_point	p1;
	int		p;

	p1.y = 0;
	while (p1.y < WIN_Y)
	{
		p1.x = 0;
		while (p1.x < WIN_X)
		{
			p = p1.x * 4 + p1.y * env->size_line;
			env->image_addr[p] = 0;
			env->image_addr[p + 1] = 0;
			env->image_addr[p + 2] = 0;
			p1.x++;
		}
		p1.y++;
	}
}

int		expose_hook(t_env *env)
{
	if (env->image == NULL)
		create_image(env);
	ft_draw_image(env);
	mlx_put_image_to_window(env->mlx, env->win, env->image, 0, 0);
	if (env->ui == 1)
	{
		display_key(env);
		ft_display_variable(env);
	}
	mlx_do_sync(env->mlx);
	ft_clean(env);
	return (0);
}

static int		ft_cnt_parts(const char *s, char c)
{
	int		cnt;
	int		in_substring;

	in_substring = 0;
	cnt = 0;
	while (*s != '\0')
	{
		if (in_substring == 1 && *s == c)
			in_substring = 0;
		if (in_substring == 0 && *s != c)
		{
			in_substring = 1;
			cnt++;
		}
		s++;
	}
	return (cnt);
}

static int		get_max_x(char **tab, t_env *env)
{
	int i;
	int	x;
	int	max_x;

	i = 0;
	max_x = 0;
	while (tab[i])
	{
		x = ft_cnt_parts(tab[i], ' ');
		max_x = max_x < x ? x : max_x;
		i++;
	}
	env->x = max_x;
	return (0);
}

static int		get_map(char **tab, t_env *env)
{
	int		**map;
	char	**tmp;
	int		i;
	int		j;

	if (check_for_char(env) == 0)
		return (0);
	get_max_x(env->tab, env);
	map = (int **)malloc(sizeof(int *) * env->y);
	i = -1;
	while (tab[++i])
	{
		j = -1;
		map[i] = (int *)malloc(sizeof(int) * env->x);
		tmp = ft_strsplit(tab[i], ' ');
		while (tmp[++j])
			map[i][j] = ft_atoi(tmp[j]);
		while (j < env->x)
		{
			map[i][j] = 0;
			j++;
		}
	}
	env->map = map;
	return (0);
}

void			read_that_file(char *filename, t_env *env)
{
	int		fd;
	char	*str;
	char	*tmp;
	int		ret;
	char	*nl;

	nl = ft_strdup("\n");
	if ((fd = open(filename, O_RDONLY)) == -1)
		return ;
	tmp = ft_strnew(0);
	env->y = 0;
	while (42)
	{
		ret = get_next_line(fd, &str);
		if (ret != 1)
			break ;
		tmp = ft_strjoin(tmp, str);
		tmp = ft_strjoin(tmp, nl);
		env->y++;
	}
	if (ret == -1)
		return ;
	else
		env->tab = ft_strsplit(tmp, '\n');
	get_map(env->tab, env);
}

int		main(int argc, char **argv)
{
	t_env	*env;

	if (argc > 1 && argc <= 3)
	{
		env = (t_env *)malloc(sizeof(t_env));
		if (ft_strcmp(argv[1], "/dev/random") == 0)
			return (0);
		read_that_file(argv[1], env);
		if (init_struct(env) == 0 || env->map == NULL)
			return (0);
		if (argc == 3)
			check_argv(argv[2], env);
		mlx_key_hook(env->win, ft_key_hook, env);
		mlx_expose_hook(env->win, expose_hook, env);
		mlx_do_sync(env->mlx);
		mlx_loop(env->mlx);
	}
	ft_putendl("Too many arguments");
	return (0);
}*/




# define H			1000
# define W			1080
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
	int			spike;

	int 		r;
	int 		g;
	int 		b;
	int 		stop;
	int 		err;
	int 		ymax;
	int 		xmax;

	t_place		*coord;
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










void	draw_color_in_image(t_vis *env, int x, int y)
{
	//t_color	u;
	int		p;

	//u.color = env->color;
	p = x * 4 + y * env->size_line;
	if (y > 0 && y < H && x > 0 && x < W)
	{
		// env->img[p] = u.rgb.b;
		// env->img[p + 1] = u.rgb.g;
		// env->img[p + 2] = u.rgb.r;
		env->drw[p] = 89;
		env->drw[p + 1] = 244;
		env->drw[p + 2] = 66;		
	}
}

void	ft_draw_line(t_place p1, t_place p2, t_vis *e)
{
	t_place		d;
	t_place		s;
	int			err;
	int			e2;

	d.x = ABS(p2.x - p1.x);
	d.y = ABS(p2.y - p1.y);
	s.x = p1.x < p2.x ? 1 : -1;
	s.y = p1.y < p2.y ? 1 : -1;
	err = (d.x > d.y ? d.x : -d.y) / 2;
	while (1)
	{
		draw_color_in_image(e, p1.x, p1.y);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		e2 = err;
		if (e2 > -d.x && ((err -= d.y) || !err))
			p1.x += s.x;
		if (e2 < d.y)
		{
			err += d.x;
			p1.y += s.y;
		}
	}
}


static void	ft_draw_y_border(t_vis *e)
{
	int		y;
	int		x;
	t_place	p1;
	t_place	p2;
	int		xx;

	y = 0;
	while (y < e->ycard - 1)
	{
		x = e->xcard - 1;
		xx = e->pos_x - (y * e->size / 2);
		//get_color(y, x, e);
		p1.y = e->pos_y + (y * e->size - (e->card[y][x] * e->spike));
		p1.x = xx + x * e->size;
		p2.y = e->pos_y + ((y + 1) * e->size - (e->card[y + 1][x] * e->spike));
		p2.x = e->pos_x - ((y + 1) * e->size / 2) + x * e->size;
		ft_draw_line(p1, p2, e);
		y++;
	}
}

static void	ft_draw_x_border(t_vis *e)
{
	int		y;
	int		x;
	t_place	p1;
	t_place p2;
	int		xx;

	y = e->ycard - 1;
	x = 0;
	xx = e->pos_x - (y * e->size / 2);
	while (x < e->xcard - 1)
	{
		//get_color(y, x, e);
		p1.y = e->pos_y + (y * e->size - (e->card[y][x] * e->spike));
		p1.x = xx + x * e->size;
		p2.y = e->pos_y + (y * e->size - (e->card[y][x + 1] * e->spike));
		p2.x = xx + (x + 1) * e->size;
		ft_draw_line(p1, p2, e);
		x++;
	}
}

void		ft_draw_border(t_vis *env)
{
	ft_draw_y_border(env);
	ft_draw_x_border(env);
}

void	vis_map(t_vis *e)
{
	int		y;
	int		x;
	t_place	p1;
	t_place	p2;
	int		xx;

	y = -1;
	printf("###OK\n");
	fprintf(stderr, "e->ycard %d e->xcard %d\n", e->ycard, e->xcard);
	while (++y < e->ycard - 1)
	{
		x = -1;
		xx = e->pos_x - (y * e->size / 2);
		while (++x < e->xcard - 1)
		{
			//get_color(y, x, e);
			p1.y = e->pos_y + (y * e->size - (e->card[y][x] * e->spike));
			p1.x = xx + x * e->size;
				fprintf(stderr, "p1.y %d p1.x %d\n", p1.y, p1.x);
			p2.y = e->pos_y + ((y + 1) * e->size - (e->card[y + 1][x] * e->spike));
			p2.x = e->pos_x - ((y + 1) * e->size / 2) + x * e->size;
			ft_draw_line(p1, p2, e);
			p2.y = e->pos_y + (y * e->size - (e->card[y][x + 1] * e->spike));
			p2.x = xx + (x + 1) * e->size;
			ft_draw_line(p1, p2, e);
		}
		//fprintf(stderr, "\n");
		ft_draw_border(e);
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


int		vis_hook(int key, t_vis *prm)
{
	if (key == 53)
	{
		//system("leaks fdf");
		exit(1);
	}
	else
		image_builder(prm);
	return (0);
}

int		build_vis(t_vis *prm)
{
	prm->mlx = mlx_init();
	prm->win = mlx_new_window(prm->mlx, W, H, "FDF");
	prm->img = mlx_new_image(prm->mlx, W, H);
	mlx_hook(prm->win, 2, 1, vis_hook, prm);
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
	prm->spike = 1;
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
		exit(1);
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



