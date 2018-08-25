/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mng_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 15:39:40 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/02 15:39:44 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

int		hex_int(char *hex)
{
	int byte;
	int val;

	val = 0;
	while (*hex)
	{
		byte = *hex++;
		if (byte >= '0' && byte <= '9')
			byte -= '0';
		else if (byte >= 'a' && byte <= 'f')
			byte -= 'a' + 10;
		else if (byte >= 'A' && byte <= 'F')
			byte -= 'A' + 10;
		val = (val << 4) | (byte & 0xF);
	}
	return (val);
}

void	set_hex(t_vis *prm, int val)
{
	prm->r = (val >> 16) & 0xFF;
	prm->g = (val >> 8) & 0xFF;
	prm->b = (val) & 0xFF;
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
