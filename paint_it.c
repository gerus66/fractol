/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint_it.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 15:41:06 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/18 14:17:18 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#define COLOR_OUT 0x00CCFCCF //0x00EFCCFC
#define COLOR_IN 0x00043EFD //0x000000FF

/*
** Change one pixel in 2D image -
** identify it by coordinates {x, y} and set it value to color.
** Need IMG_SIZE value for not drowing outside image.
*/

static void		put_pixel_to_img(void *img, int x, int y, int color)
{
	int		bits;
	int		s_line;
	int		endian;
	int		i;
	char	*data;

//	if (x < 0 || y < 0 || x >= IMG_SIZE || y >= IMG_SIZE)
//		return ;
	data = mlx_get_data_addr(img, &bits, &s_line, &endian);
	data += s_line * y;
	data += bits / 8 * x;
	i = -1;
	while (++i < bits / 8)
	{
		*data = color;
		color = color >> 8;
		data++;
	}
}

#define N_VAL 50000

static int	m_color(void *mlx, int n)
{
	t_color	color;
	t_color	out;
	t_color	in;
	int		i;

	if (n == 1 || n == 0)
		return (mlx_get_color_value(mlx, 0));
	if (n > N_VAL)
		return (mlx_get_color_value(mlx, COLOR_OUT));
	color.cl = 0;
	out.cl = COLOR_OUT;
	in.cl = COLOR_IN;
	i = -1;
	while (++i < 4)
		color.cl4[i] = in.cl4[i] + (out.cl4[i] - in.cl4[i]) / (N_VAL / n);
	return (mlx_get_color_value(mlx, color.cl));
}

//#include <stdio.h>
void	paint_it(int *map, t_imgbox *ibox)
{
	int	i;

	i = -1;
	while (++i < IMG_SIZE * IMG_SIZE)
	{
	//	if (map[i])
	//		printf("%d ", i);
		put_pixel_to_img(ibox->img, i % IMG_SIZE, i / IMG_SIZE, 
			m_color(ibox->mlx, map[i]));
	//		mlx_get_color_value(ibox->mlx, map[i] ? COLOR_IN : COLOR_OUT));
	}
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->img, WND_W - IMG_SIZE, 
			WND_H - IMG_SIZE);
}
