/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/02 15:42:27 by amakhiny          #+#    #+#             */
/*   Updated: 2018/03/02 15:42:30 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/fdf.h"

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

int		map_check_char(char *out)
{
	while (*out)
	{
		if (*out == ',')
		{
			if (!valid_hex(out))
				return (0);
			out++;
			while ((*out >= 65 && *out <= 70) ||
			(*out >= 97 && *out <= 102) ||
			(*out >= 48 && *out <= 57) || *out == 'x')
				out++;
		}
		else if (((*out >= 48 && *out <= 57) || (*out >= 65 && *out
			<= 70) || (*out >= 97 && *out <= 102) || *out == ' ' ||
			*out == '\n' || *out == '-'))
			out++;
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
		if (out[i] == '-' && (out[i + 1] < 48 || out[i + 1] > 57))
			return (0);
	}
	return (space);
}

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
		if (!map_check_char(output))
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
