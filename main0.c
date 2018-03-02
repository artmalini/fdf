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

//# define H			1000
//# define W			1600
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
# define ABS(v)		((v) > 0 ? (v) : -(v))

typedef struct			s_point
{
	int					x;
	int					y;
}						t_place;

typedef struct 	s_viz
{
	int 		**card;
	int			**hex;
	int 		ycard;
	int 		xcard;
	int			size;
	int			size_line;
	int			pos_x;
	int			pos_y;	
	double		zoom;

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

int		valid_hex(char *str)
{
	str++;
	if (*str == '0')
		str++;
	if (*str == 'x')
		str++;
	while ((*str >= 65 && *str <= 70) ||
			(*str >= 97 && *str <= 102) ||
			(*str >= 48 && *str <= 57))
		str++;
	str--;
	if ((*str >= 65 && *str <= 70) ||
			(*str >= 97 && *str <= 102) ||
			(*str >= 48 && *str <= 57))
		return (1);
	return (0);
}

int		map_check_char(char *out, int val)
{
	int		k;

	k = 0;
	while (*out)
	{
		if (*out == ',')
		{
			//printf("map y %d x %d out %s\n", val, k, out);
			if (!valid_hex(out))
				return (0);
			out++;
			while ((*out >= 65 && *out <= 70) ||
			(*out >= 97 && *out <= 102) ||
			(*out >= 48 && *out <= 57) || *out == 'x')
			{
				//printf("out%s\n", out);
				out++;
				k++;
				//printf("k %d\n", k);
			}
			//out--;
			//printf("out%s\n", out);
		}
		else if (((*out >= 48 && *out <= 57) || (*out >= 65 && *out
			<= 70) || (*out >= 97 && *out <= 102) || *out == ' ' ||
			*out == '\n'))
		{
			out++;
			k++;
		}
		else
			return (0);
	}
	return (1);
}

int		map_check2(char *out)
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

/*int		get_max_x(t_vis *prm, char *output)
{
	int		val;

	val = map_check2(output);
	//prm->xcard = val;
	if (val > prm->xcard)
		return (val);
	else
		return (prm->xcard);
}*/

int		map_check(int fd)
{
	int		val;
	int		space;
	char	*output;

	val = 0;
	space = 0;
	output = NULL;
	while (get_next_line(fd, &output) > 0)
	{
		space = map_check2(output);
		if (!map_check_char(output, val))
			space = 0;
		if (space == 0)
		{
			free(output);
			return (0);
		}
		space = 0;
		val++;
		free(output);
	}
	if (output != NULL)
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
		//printf("tmp[k] %s\n", tmp[k]);
		prm->card[i][k] = ft_atoi(tmp[k]);
		//fprintf(stderr, "tmp[k] %s prm->card[i][k] %d", tmp[k], prm->card[i][k]);
		//fprintf(stderr, "%d", prm->card[i][k]);
	}
	free_map(tmp, xsize);
	free_map(out, 1);
	return (0);
}


int hex_int(char *hex)
{
	int byte;
	int val;

	val = 0;
	while (*hex)
	{
		byte = *hex++; 
		if (byte >= '0' && byte <= '9')
			byte -= '0';
		else if (byte >= 'a' && byte <='f')
			byte -= 'a' + 10;
		else if (byte >= 'A' && byte <='F')
			byte -= 'A' + 10; 
		val = (val << 4) | (byte & 0xF);
	}
	//printf("val|%d\n", val);
	return (val);
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
	char	**tmp;
	char	**hexim;

	prm->card = (int **)malloc(sizeof(int *) * prm->ycard);
	prm->hex = (int **)malloc(sizeof(int *) * prm->ycard);
	build_card_pars(prm, fd, output, 0);
	/*while (get_next_line(fd, &output) > 0)
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
	free(output);*/
}









/*void convhex(char *mas)
{
	int	i;
	int temp;

	i = 0;
	while (mas[i] != '\0')
	{
		temp = (mas[i] - '0') % 16 ;
		temp = temp / 16 ;

		ft_putchar(temp + '0');
		i++;
	}		
}*/

/*int hex2int(t_vis *prm, char *hex)
{
	int byte;
    int val;

    val = 0;
    while (*hex)
    {
         byte = *hex++; 
        if (byte >= '0' && byte <= '9')
        	byte = byte - '0';
        else if (byte >= 'a' && byte <='f')
        	byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F')
        	byte = byte - 'A' + 10; 
        val = (val << 4) | (byte & 0xF);
    }
    printf("val %d\n", val);

    prm->r = ((val >> 16) & 0xFF);
	prm->g = ((val >> 8) & 0xFF);
	prm->b = ((val) & 0xFF);
    //return val;
    printf("prm->r %d prm->g %d prm->b %d\n", prm->r, prm->g, prm->b);
    return (0);
}*/

void	set_hex(t_vis *prm, int val)
{
	prm->r = ((val >> 16) & 0xFF);
	prm->g = ((val >> 8) & 0xFF);
	prm->b = ((val) & 0xFF);
}

void	set_color(t_vis *prm, int r, int g, int b)
{
	prm->r = r;
	prm->g = g;
	prm->b = b;
}

void	fit_color(t_vis *prm, int x, int y)
{
	double		val;
	double		val1;
	intmax_t	i;

	i = 1;
	val = prm->card[y][x] * prm->zoom;
	val1 = prm->hex[y][x] * prm->zoom;
	i *= val;
	if (val != val1)
	{
		set_hex(prm, prm->hex[y][x]);
		return ;
	}
	else if (i == 0)
		set_color(prm, 55, 244, 89);
	else if (i < 0)
		set_color(prm, 66, 128, 244);
	else if (i > 0 && i < 200)
		set_color(prm, 96, 66, 49);
	else
		set_color(prm, 255, 255, 255);
}




void	fdf_color(t_vis *prm, int x, int y)
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
	//printf("fdf\n");
}

void	color_paint(t_place v0, t_place v1, t_vis *prm)
{
	t_place		d;
	t_place		s;
	int			err;
	int			e2;

	//printf("one\n");
	d.x = ABS(v1.x - v0.x);
	d.y = ABS(v1.y - v0.y);
	s.x = v0.x >= v1.x ? -1 : 1;
	s.y = v0.y >= v1.y ? -1 : 1;
	err = (d.x > d.y ? d.x : -d.y) / 2;
	//fprintf(stderr, "err %d\n", err);
	while (1)
	{	
		fdf_color(prm, v0.x, v0.y);
		if (v0.y == v1.y && v0.x == v1.x)
			break ;		
		e2 = err;
			//fprintf(stderr, " v0.x %d v1.x %d v0.y %d v1.y %d e2 %d d.y %d\n", v0.x, v1.x, v0.y, v1.y, e2, d.y);
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
	//printf("two\n");
}


void	border_y(t_vis *prm)
{
	int		y;
	int		x;
	t_place	v0;
	t_place	v1;
	//int		xx;

	y = -1;
	while (++y < prm->ycard - 1)
	{
		x = prm->xcard - 1;
		//xx = prm->pos_x - (y * prm->size / 2);
		//get_color(y, x, e);
		v0.y = prm->pos_y + (y * prm->size - (prm->card[y][x] * prm->zoom));
		v0.x = (prm->pos_x - (y * prm->size / 2)) + x * prm->size;
		v1.y = prm->pos_y + ((y + 1) * prm->size - (prm->card[y + 1][x] * prm->zoom));
		v1.x = prm->pos_x - ((y + 1) * prm->size / 2) + x * prm->size;
		color_paint(v0, v1, prm);
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
	x = -1;
	//xx = prm->pos_x - (y * prm->size / 2);
	while (++x < prm->xcard - 1)
	{
		//get_color(y, x, e);
		v0.y = prm->pos_y + (y * prm->size - (prm->card[y][x] * prm->zoom));
		v0.x = (prm->pos_x - (y * prm->size / 2)) + x * prm->size;
		v1.y = prm->pos_y + (y * prm->size - (prm->card[y][x + 1] * prm->zoom));
		v1.x = (prm->pos_x - (y * prm->size / 2)) + (x + 1) * prm->size;
		color_paint(v0, v1, prm);
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
			v0.y = prm->pos_y + (i * prm->size - (prm->card[i][j] * prm->zoom));
			v0.x = prm->pos_x - (i * prm->size / 2) + j * prm->size;
			//printf("ok\n");
			// v0.y = v0.y * cos(angle) + v0.x * sin(angle);
			// v0.x = v0.x * cos(angle) - v0.y * sin(angle);
			// v0.y = round(v0.y);
			// v0.x = round(v0.x);
			//v0.y = prm->pos_y + (i * prm->size - (prm->card[i][j] * prm->zoom));
			//v0.x = (prm->pos_x - (i * prm->size / 2)) + j * prm->size;
				//fprintf(stderr, "v0.y %d v0.x %d\n", v0.y, v0.x);
			angle = (3.14159265 * prm->angl_y) / 180;
			v1.y = prm->pos_y + ((i + 1) * prm->size - (prm->card[i + 1][j] * prm->zoom));
			v1.x = prm->pos_x - ((i + 1) * prm->size / 2) + j * prm->size;
			// v1.y = v1.y * cos(angle) - v1.x * sin(angle);
			// v1.x = v1.x * cos(angle) + v1.y * sin(angle);
			// v1.y = round(v1.y);
			// v1.x = round(v1.x);
				//fprintf(stderr, "(i + 1) v0.y %d v0.x %d\n", v0.y, v0.x);
			color_paint(v0, v1, prm);

			angle = (3.14159265 * prm->angl_z) / 180;
			v1.y = prm->pos_y + (i * prm->size - (prm->card[i][j + 1] * prm->zoom));
			v1.x = prm->pos_x - (i * prm->size / 2) + (j + 1) * prm->size;
				//fprintf(stderr, " (j + 1) v0.y %d v0.x %d\n", v0.i, v0.j);
			// v1.y = v1.y * cos(angle) + v1.x * sin(angle);
			// v1.x = v1.x * cos(angle) - v1.y * sin(angle);
			// v1.y = round(v1.y);
			// v1.x = round(v1.x);			
			color_paint(v0, v1, prm);
		}
		//fprintf(stderr, "\n");
		color_paint_border(prm);
	}
}


void	print_help(t_vis *prm)
{
	mlx_string_put(prm->mlx, prm->win, 10, 10, 0xf4d942, "ESC to quit");
	mlx_string_put(prm->mlx, prm->win, 10, 35, 0xf4d942, "Use arrows to move");
	mlx_string_put(prm->mlx, prm->win, 10, 65, 0xf4d942, "Mouse whell to resize altitude");
	mlx_string_put(prm->mlx, prm->win, 10, 95, 0xf4d942, "Keys to scale + - ");
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
	print_help(prm);
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
	//fprintf(stderr, "key %d\n", key);
	if (key == 53)
	{
		//system("leaks fdf");
		exit(EXIT_SUCCESS);
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
	prm->pos_x = W / 4;
	prm->pos_y = H / 2;
	prm->zoom = 2;
	prm->r = 0;
	prm->g = 0;
	prm->b = 0;
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
	if(!(prm->ycard = map_check(fd)))
	{
		print_err("Wrong map\n");
		exit(1);
	}
	close(fd);
	fd = open(arg[1], O_RDONLY);
	build_card(prm, fd, output);
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



