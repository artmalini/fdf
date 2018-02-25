# include <mlx.h>
# include "math.h"
# include "libft/libft.h"
# include "includes/get_next_line.h"
# define WIDTH 2560
# define HEIGHT 1440
# define ECHAP	53
# define UP		126
# define DOWN	125
# define RIGHT	124
# define LEFT	123
# define ZOOM	69
# define DEZOOM	78

typedef struct		s_point
{
	int				x;
	int				y;
	int				z;
	int				size_x;
	int				size_y;
}					t_point;

typedef struct		s_af
{
	int				s_line;
	void			*win;
	void			*mlx;
	void			*img;
	t_point			***point;
	int				deep;
	unsigned int	tight;
	unsigned int	tight2;
	int				zoom;
	int				persp;
	int				bits_per_pixel;
	int				size_line;
	int				endian;
	char			*ret;
	int				tmpz;
	int				tmpi;
	int				tmpj;
	int				couleur;
	int				r;
	int				v;
	int				b;
	int				min_hight;
	int				max_hight;
	int				degra;
	int				opacity;

}					t_af;

typedef struct		s_map
{
	char			**argv;
	char			**map;
	char			**tmp;
	int				x;
	int				y;
}					t_map;

int					ft_key_hook(int keycode, t_af *af);
int					ft_key_hook2(int keycode, t_af *af);
int					ft_key_hook3(int keycode, t_af *af);
int					ft_key_hook4(int keycode, t_af *af);
void				ft_degra2(t_af *af, int q);
void				ft_exit_error(void);
void				ft_print_info(t_af *p);
void				ft_print_info2(t_af *p, char *str);
void				ft_reset(t_af *af);
unsigned int		ft_color(t_af *af, int i2, int j);
void				ft_count_size2(int tmp, int spaces);
void				ft_exit_error();
static int			ft_loop_key_hook(t_af *af);
void				ft_reset(t_af *af);
void				ft_get_map(struct s_map *map);
t_point				***ft_get_coord(struct s_map *map);
int					ft_build_mlx(struct s_af *p);
int					ft_expose_hook(t_af *p);
void				ft_choose_persp(t_af *af, int i, int j);
void				ft_iso_persp(t_af *af, int i, int j);
void				ft_print_info(t_af *p);
unsigned int		ft_color(t_af *af, int i, int j);
int					ft_key_hook(int keycode, t_af *af);
int					ft_mouse_hook(int button, int x, int y, t_af *af);
int					ft_mlx_event(struct s_af *af);
void				ft_put_point(struct s_af *af);









void				ft_degra2(t_af *af, int q)
{
	if (af->degra == 0)
		return ;
	else if (af->degra == 1)
		af->b = q;
	else if (af->degra == 2)
		af->r = q;
	else if (af->degra == 3)
		af->v = q;
}


void			ft_my_pixel_put(t_af *af, int i, int j, float q)
{
	int			k;

	ft_degra2(af, q);
	if ((i > 0 && j > 0 && i < WIDTH && j < HEIGHT))
	{
		af->ret[((i * 4 + (j * af->size_line)))] = af->b;
		af->ret[((i * 4 + (j * af->size_line))) + 1] = af->r;
		af->ret[((i * 4 + (j * af->size_line))) + 2] = af->v;
		af->ret[((i * 4 + (j * af->size_line))) + 3] = af->opacity;
	}
}

float			ft_degra(t_af *af, t_point *b, t_point *a, int i)
{
	float		ret;
	float		dif;
	float		alt;

	dif = (float)(i - a->x) / (float)(b->x - a->x);
	alt = (float)(a->z + ((b->z - a->z) * dif)) / (float)(af->max_hight - af->min_hight);
	ret = 255 * alt;
	return (ret);
}

void			ft_wire(t_af *af, t_point *b, t_point *a)
{
	int			i;
	int			j;

	if ((b->x - a->x) == 0)
		return ;
	i = a->x;
	j = ((b->y - a->y) * (i - a->x)) / (b->x - a->x) + a->y;
	while (i < b->x)
	{
		while (j <= ((b->y - a->y) * (i + 1 - a->x) / (b->x - a->x)) + a->y)
			ft_my_pixel_put(af, i, j++, ft_degra(af, b, a, i));
		while (j > ((b->y - a->y) * (i + 1 - a->x) / (b->x - a->x)) + a->y)
			ft_my_pixel_put(af, i, j--, ft_degra(af, b, a, i));
		i++;
	}
}

void			ft_iso_persp(t_af *af, int i, int j)
{
	int			x;
	int			y;
	t_point		***c;

	c = af->point;
	c[i][j]->x = (af->zoom * (i + j) / (c[i][j]->size_x + c[i][j]->size_y)) + af->tight2;
	c[i][j]->y = ((af->zoom * (c[i][j]->size_x + i - j) / (c[i][j]->size_x + c[i][j]->size_y) - (c[i][j]->z) * af->deep) + af->tight);
	c[i][j]->y = c[i][j]->y / 2;
	if (j > 0)
		ft_wire(af, c[i][j], c[i][j - 1]);
	if (i > 0)
		ft_wire(af, c[i][j], c[i - 1][j]);
}

void			ft_choose_persp(t_af *af, int i, int j)
{
	if (af->persp == 1)
		ft_iso_persp(af, i, j);
}










void				ft_exit_error(void)
{
	ft_putstr("Invalid map\n");
	exit(1);
}

void				ft_print_info3(t_af *p, char *str)
{
	if (p->degra > 0)
	{
		str = "Degrade sur le ";
		mlx_string_put(p->mlx, p->win, 40, 190, 0x0FFFFFF, str);
		if (p->degra == 1)
		{
			str = "bleu";
			mlx_string_put(p->mlx, p->win, 200, 190, 0x0FFFFFF, str);
		}
		if (p->degra == 2)
		{
			str = "vert";
			mlx_string_put(p->mlx, p->win, 200, 190, 0x0FFFFFF, str);
		}
		if (p->degra == 3)
		{
			str = "rouge";
			mlx_string_put(p->mlx, p->win, 200, 190, 0x0FFFFFF, str);
		}
	}
	if (p->degra == 0)
	{
		str = "Pas de degrade";
		mlx_string_put(p->mlx, p->win, 40, 190, 0x0FFFFFF, str);
	}
}

void				ft_print_info2(t_af *p, char *str)
{
	str = "clic droit :";
	mlx_string_put(p->mlx, p->win, 40, 110, 0x0FFFFFF, str);
	str = " changer les couleurs:";
	mlx_string_put(p->mlx, p->win, 170, 110, 0x0FFFFFF, str);
	str = " v:";
	mlx_string_put(p->mlx, p->win, 385, 110, 0x0FFFFFF, str);
	str = ft_itoa(p->r);
	mlx_string_put(p->mlx, p->win, 420, 110, 0x0FFFFFF, str);
	str = " r:";
	mlx_string_put(p->mlx, p->win, 385, 130, 0x0FFFFFF, str);
	str = ft_itoa(p->v);
	mlx_string_put(p->mlx, p->win, 420, 130, 0x0FFFFFF, str);
	str = " b:";
	mlx_string_put(p->mlx, p->win, 385, 150, 0x0FFFFFF, str);
	str = ft_itoa(p->b);
	mlx_string_put(p->mlx, p->win, 420, 150, 0x0FFFFFF, str);
	str = "Opacite :";
	mlx_string_put(p->mlx, p->win, 40, 230, 0x0FFFFFF, str);
	str = ft_itoa(p->opacity);
	mlx_string_put(p->mlx, p->win, 420, 230, 0x0FFFFFF, str);
	ft_print_info3(p, str);
}

void				ft_print_info(t_af *p)
{
	char			*str;

	str = "Echap      :";
	mlx_string_put(p->mlx, p->win, 40, 10, 0x0FFFFFF, str);
	str = " quitter le programme";
	mlx_string_put(p->mlx, p->win, 170, 10, 0x0FFFFFF, str);
	str = "+ / -      :";
	mlx_string_put(p->mlx, p->win, 40, 30, 0x0FFFFFF, str);
	str = " zoom avant / arriere:";
	mlx_string_put(p->mlx, p->win, 170, 30, 0x0FFFFFF, str);
	str = ft_itoa(p->zoom);
	mlx_string_put(p->mlx, p->win, 420, 30, 0x0FFFFFF, str);
	str = "molette    :";
	mlx_string_put(p->mlx, p->win, 40, 70, 0x0FFFFFF, str);
	str = " modifier l'altitude:";
	mlx_string_put(p->mlx, p->win, 170, 70, 0x0FFFFFF, str);
	str = ft_itoa(p->deep);
	mlx_string_put(p->mlx, p->win, 420, 70, 0x0FFFFFF, str);
	str = "Delet:        reinitaliser la map";
	mlx_string_put(p->mlx, p->win, 40, 270, 0x0FFFFFF, str);
	ft_print_info2(p, str);
}


int					ft_key_hook(int keycode, t_af *af)
{
	int				i;
	int				j;

	i = 1;
	if (keycode == ECHAP)
		exit(1);
	if (keycode == UP)
		af->tight -= 30;
	if (keycode == DOWN)
		af->tight += 30;
	if (keycode == RIGHT)
		af->tight2 += 30;
	if (keycode == LEFT)
		af->tight2 -= 30;
	if (keycode == ZOOM)
	{
		af->zoom += 50;
		af->tight2 -= 25;
		af->tight -= 25;
	}
	if (keycode == 117)
		ft_reset(af);
	return (ft_key_hook2(keycode, af));
}


int					ft_key_hook2(int keycode, t_af *af)
{
	if (keycode == DEZOOM)
	{
		af->zoom -= 50;
		af->tight2 += 25;
		af->tight += 25;
	}
	if (keycode == 86)
	{
		if ((af->r -= 10) < 0)
			af->r = 0;
		else
			af->r -= 10;
	}
	if (keycode == 87)
	{
		if ((af->v -= 10) < 0)
			af->v = 0;
		else
			af->v -= 10;
	}
	if (keycode == 89)
		af->degra = 1;
	if (keycode == 91)
		af->degra = 2;
	return (ft_key_hook3(keycode, af));
}

int					ft_key_hook3(int keycode, t_af *af)
{
	if (keycode == 88)
	{
		if ((af->b -= 10) < 0)
			af->b = 0;
		else
			af->b -= 10;
	}
	if (keycode == 92)
		af->degra = 3;
	if (keycode == 83)
	{
		if ((af->r += 10) > 255)
			af->r = 255;
		else
			af->r += 10;
	}
	if (keycode == 84)
	{
		if ((af->v += 10) > 255)
			af->v = 255;
		else
			af->v += 10;
	}
	return (ft_key_hook4(keycode, af));
}



int					ft_key_hook4(int keycode, t_af *af)
{
	if (keycode == 85)
	{
		if ((af->b += 10) > 255)
			af->b = 255;
		else
			af->b += 10;
	}
	if (keycode == 82 && (af->opacity + 10) <= 255)
		af->opacity += 10;
	if (keycode == 65 && (af->opacity - 10) >= 0)
		af->opacity -= 10;
	if (keycode == 18)
	{
		ft_reset(af);
		af->persp = 1;
	}
	ft_loop_key_hook(af);
	return (0);
}



void				ft_put_point(t_af *af)
{
	int				i;
	int				j;

	i = 0;
	j = 0;
	while (i < af->point[0][0]->size_y)
	{
		j = 0;
		while (j < af->point[i][0]->size_x)
			ft_choose_persp(af, i, j++);
		i++;
	}
}


static int			ft_loop_key_hook(t_af *af)
{
	af->ret = mlx_get_data_addr(af->img, &(af->bits_per_pixel),
		&(af->size_line), &(af->endian));
	ft_put_point(af);
	mlx_put_image_to_window(af->mlx, af->win, af->img, 0, 0);
	mlx_destroy_image(af->mlx, af->img);
	af->img = mlx_new_image(af->mlx, WIDTH, HEIGHT);
	ft_print_info(af);
	return (0);
}


int					ft_mouse_hook(int button, int x, int y, t_af *af)
{
	if (button == 4)
		af->deep += 1;
	if (button == 5)
		af->deep -= 1;
	if (button == 1)
	{
		if ((af->r + 10) <= 255)
			af->r += 10;
		if ((af->v + 10) <= 255)
			af->v += 10;
		if ((af->b + 10) <= 255)
			af->b += 10;
	}
	if (button == 2)
	{
		if ((af->r - 10) >= 0)
			af->r -= 10;
		if ((af->v - 10) >= 0)
			af->v -= 10;
		if ((af->b - 10) >= 0)
			af->b -= 10;
	}
	ft_loop_key_hook(af);
	return (0);
}






void				ft_count_size2(int tmp, int spaces)
{
	if (tmp != 0 && (tmp != spaces))
		ft_exit_error();
}


int					ft_count_size(int fd)
{
	char			*line;
	int				count;
	int				spaces;
	int				tmp;
	int				i;

	tmp = 0;
	count = 0;
	while (get_next_line(fd, &line) != 0)
	{
		spaces = 0;
		i = 0;
		while (line[i] && line[0] != '\0')
		{
			if (((line[i] >= 48 && line[i] <= 57) || (line[i] >= 65 && line[i]
				<= 90) || (line[i] >= 97 && line[i] <= 122)) &&
				(line[i + 1] == ' ' || line[i + 1] == '\0'))
				spaces++;
			i++;
		}
		count++;
		ft_count_size2(tmp, spaces);
		tmp = spaces;
	}
	return (count);
}

void				ft_get_map(struct s_map *map)
{
	int				i;
	int				fd;
	char			*line;

	i = 0;
	map->y = 0;
	fd = open(map->argv[1], O_RDONLY);
	if (fd == -1)
	{
		ft_putstr("Invalid argument\n");
		exit(1);
	}
	map->y = ft_count_size(fd);
	close(fd);
	fd = open(map->argv[1], O_RDONLY);
	map->map = (char **)malloc(sizeof(char *) * map->y + 1);
	while ((get_next_line(fd, &line)) != 0)
		if (line[0] != '\0')
			map->map[i++] = line;
		else
			map->y--;
	close(fd);
}

static	t_point		*ft_new_coord(int x, int y, int z, t_map *map)
{
	t_point			*point;

	if (!(point = (t_point *)malloc(sizeof(t_point))))
		return (NULL);
	point->size_x = (map->x);
	point->size_y = (map->y);
	point->z = z;
	point->x = x - point->size_x / 2;
	point->y = y - map->y / 2;
	return (point);
}

void				ft_stock_coord(int i, int i2, t_point ***point, t_map *map)
{
	while (i2 < map->x)
	{
		if (map->tmp[i2] && map->tmp[i2][0] != '\n')
			point[i][i2] = ft_new_coord(i2, i, ft_atoi(map->tmp[i2]), map);
		i2++;
	}
	free(map->tmp);
}

t_point				***ft_get_coord(struct s_map *map)
{
	int				i;
	int				i2;
	t_point			***point;

	i = 0;
	ft_get_map(map);
	if (!(point = (t_point ***)malloc(sizeof(t_point **) * map->y + 1)))
		return (NULL);
	while (i < map->y)
	{
		map->tmp = ft_strsplit(map->map[i], ' ');
		map->x = 0;
		while (map->tmp[map->x])
			map->x++;
		i2 = 0;
		if (!(point[i] = (t_point **)malloc(sizeof(t_point *) * (map->x + 1))))
			return (NULL);
		ft_stock_coord(i, i2, point, map);
		i++;
	}
	map->tmp = NULL;
	return (point);
}




void	ft_init(t_af *p)
{
	p->deep = 0;
	p->tight = 0;
	p->tight2 = 0;
	p->zoom = WIDTH;
	p->persp = 1;
	p->r = 255;
	p->v = 255;
	p->b = 255;
	p->degra = 0;
}

int		ft_build_mlx(t_af *p)
{
	ft_init(p);
	p->mlx = mlx_init();
	p->win = mlx_new_window(p->mlx, WIDTH, HEIGHT, "fdf");
	p->img = mlx_new_image(p->mlx, WIDTH, HEIGHT);
	mlx_hook(p->win, 2, 2, ft_key_hook, p);
	mlx_mouse_hook(p->win, ft_mouse_hook, p);
	mlx_loop(p->mlx);
	return (0);
}


void				ft_reset(t_af *af)
{
	af->deep = 0;
	af->tight = 0;
	af->tight2 = 0;
	af->zoom = WIDTH;
	af->persp = 1;
	af->r = 255;
	af->v = 255;
	af->b = 255;
	af->opacity = 0;
	af->degra = 0;
}

oid				ft_get_map_hight(t_af *af)
{
	int				i;
	int				j;

	i = 0;
	j = 0;
	while (i < af->point[0][0]->size_y)
	{
		j = 0;
		while (j < af->point[i][0]->size_x)
		{
			if (af->point[i][j]->z > af->max_hight)
				af->max_hight = af->point[i][j]->z;
			if (af->point[i][j]->z < af->min_hight)
				af->min_hight = af->point[i][j]->z;
			j++;
		}
		i++;
	}
}

int					main(int argc, char **argv)
{
	t_map			*map;
	t_af			*af;

	if (argc != 2)
	{
		ft_putstr("Invalide argument\n");
		return (0);
	}
	af = (t_af *)malloc(sizeof(t_af));
	map = (t_map *)malloc(sizeof(t_map));
	map->argv = argv;
	af->point = ft_get_coord(map);
	ft_get_map_hight(af);
	ft_build_mlx(af);
	free(map);
	free(af->point);
	free(af);
	return (0);
}