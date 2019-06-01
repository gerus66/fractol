/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 04:21:34 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/27 05:59:51 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static int		scale_it(int key, int x1, int x2, void *param)
{
	t_kernbox	*kbox;
	t_quebox	*qbox;
	t_imgbox	*ibox;

	kbox = (t_kernbox *)(((int **)param)[0]);
	qbox = (t_quebox *)(((int **)param)[1]);
	ibox = (t_imgbox *)(((int **)param)[2]);
	if (key == 5)
	{
		kbox->f[0] += (double)(x1 - HELP_W) * kbox->f[2] * (1 - SC_COEF);
		kbox->f[1] -= (double)x2 * kbox->f[2] * (1 - SC_COEF);
		kbox->f[2] *= SC_COEF;
	}
	else
	{
		kbox->f[2] /= SC_COEF;
		kbox->f[0] += (double)(x1 - HELP_W) * kbox->f[2] * (SC_COEF - 1);
		kbox->f[1] -= (double)x2 * kbox->f[2] * (SC_COEF - 1);
	}
	kbox->i[0] = depth(kbox->f[2]);
	if (kbox->i[0] > MAX_DEPTH)
		kbox->i[0] = (double)MAX_DEPTH;
	reprint_all(kbox, qbox, ibox);
	return (0);
}

static int		click_mouse(int key, int x1, int x2, void *param)
{
	t_kernbox	*kbox;
	t_quebox	*qbox;
	t_imgbox	*ibox;

	if ((key == 4 || key == 5) && x1 > HELP_W)
		return (scale_it(key, x1, x2, param));
	kbox = (t_kernbox *)(((int **)param)[0]);
	qbox = (t_quebox *)(((int **)param)[1]);
	ibox = (t_imgbox *)(((int **)param)[2]);
	if (key == 1 && x1 > HELP_W && x2 > 0)
		ibox->mouse_left = 1;
	else if (key == 2 && x1 > HELP_W && x2 > 0)
		ibox->mouse_right = 1;
	if ((key == 1 || key == 2) && x1 > HELP_W && x2 > 0 &&
			(ibox->mouse_pos[1] = x2))
		ibox->mouse_pos[0] = x1;
	if (key == 1 && x1 < 150 && x2 > 20 && x2 < 45)
		init_fractol(kbox, qbox, ibox, MAND);
	else if (key == 1 && x1 < 150 && x2 > 45 && x2 < 75)
		init_fractol(kbox, qbox, ibox, JUL);
	else if (key == 1 && x1 < 150 && x2 > 75 && x2 < 100)
		init_fractol(kbox, qbox, ibox, SHIP);
	if (key == 1 && x1 < 150 && x2 > 20 && x2 < 100)
		reprint_all(kbox, qbox, ibox);
	return (0);
}

static int		release_mouse(int key, int x1, int x2, void *param)
{
	t_kernbox	*kbox;
	t_quebox	*qbox;
	t_imgbox	*ibox;

	if (key)
		x1 = x2;
	kbox = (t_kernbox *)(((int **)param)[0]);
	qbox = (t_quebox *)(((int **)param)[1]);
	ibox = (t_imgbox *)(((int **)param)[2]);
	ibox->mouse_left = 0;
	ibox->mouse_right = 0;
	return (0);
}

static int		move_mouse(int x1, int x2, void *param)
{
	t_kernbox	*kbox;
	t_quebox	*qbox;
	t_imgbox	*ibox;

	kbox = (t_kernbox *)(((int **)param)[0]);
	qbox = (t_quebox *)(((int **)param)[1]);
	ibox = (t_imgbox *)(((int **)param)[2]);
	if (ibox->mouse_left)
	{
		kbox->f[0] -= kbox->f[2] * (x1 - ibox->mouse_pos[0]);
		kbox->f[1] += kbox->f[2] * (x2 - ibox->mouse_pos[1]);
		ibox->mouse_pos[0] = x1;
		ibox->mouse_pos[1] = x2;
		reprint_all(kbox, qbox, ibox);
	}
	if (ibox->mouse_right)
	{
		kbox->f[3] += kbox->f[2] * (x1 - ibox->mouse_pos[0]);
		kbox->f[4] -= kbox->f[2] * (x2 - ibox->mouse_pos[1]);
		ibox->mouse_pos[0] = x1;
		ibox->mouse_pos[1] = x2;
		reprint_all(kbox, qbox, ibox);
	}
	return (0);
}

void			hook_mouse(void *wnd, void *param)
{
	mlx_hook(wnd, 4, 1, click_mouse, param);
	mlx_hook(wnd, 5, 1, release_mouse, param);
	mlx_hook(wnd, 6, 1, move_mouse, param);
}
