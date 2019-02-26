/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 15:47:17 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/26 09:43:30 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	clean_all(t_kernbox *kbox, t_quebox *qbox, char *msg)
{
	ft_putstr(msg);
	if (kbox)
	{
		clReleaseKernel(kbox->kern);
		clReleaseMemObject(kbox->map_buf);
		free(kbox->map);
	}
	if (qbox)
	{
		clReleaseContext(qbox->ctx);
		clReleaseCommandQueue(qbox->queue);
		clReleaseProgram(qbox->prog);
	}
	exit(0);
}


static double depth(float sc)
{
	sc = 1 / sc;
//	return (sqrt(sc) * log10(sqrt(sc - 100)) + 100);
//	return (sqrt(sc / 10.0 + 1000000000.0) / 100);
	if (sc < 250)
		sc = 250;
	return (1000.0 * log10(sc - 248.0));
//	return (200.0 * log((sc - 250.0) / 100.0 + 2.0));
}

static void	init_mand(t_kernbox *kbox, t_quebox *qbox, t_imgbox *ibox)
{
	init_kern(kbox, MAND, qbox, ibox);
	kbox->f[0] = MAND_START_X;
	kbox->f[1] = MAND_START_Y;
	kbox->f[2] = MAND_START_SC;
	kbox->i[0] = depth(kbox->f[2]);
	kbox->i[1] = 1;
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 20);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 50);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 80);
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 20, CUR_COLOR, "Mandelbrot");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 50, HELP_COLOR, "Julia");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 80, HELP_COLOR, "Burning ship");
}

static void	init_jul(t_kernbox *kbox, t_quebox *qbox, t_imgbox *ibox)
{
	init_kern(kbox, JUL, qbox, ibox);
	kbox->f[0] = MAND_START_X;
	kbox->f[1] = MAND_START_Y;
	kbox->f[2] = MAND_START_SC;
	kbox->i[0] = depth(kbox->f[2]);
	kbox->i[1] = 1;
	kbox->f[3] = JUL_START_X;
	kbox->f[4] = JUL_START_Y;
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 20);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 50);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 80);
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 20, HELP_COLOR, "Mandelbrot");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 50, CUR_COLOR, "Julia");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 80, HELP_COLOR, "Burning ship");
}

static void	init_ship(t_kernbox *kbox, t_quebox *qbox, t_imgbox *ibox)
{
	init_kern(kbox, SHIP, qbox, ibox);
	kbox->f[0] = MAND_START_X;
	kbox->f[1] = MAND_START_Y;
	kbox->f[2] = MAND_START_SC;
	kbox->i[0] = depth(kbox->f[2]);
	kbox->i[1] = 1;
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 20);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 50);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 80);
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 20, HELP_COLOR, "Mandelbrot");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 50, HELP_COLOR, "Julia");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 80, CUR_COLOR, "Burning ship");
}

/*
** handle keyboard events
*/

static int	keyboard(int key, void *param)
{
	t_kernbox	*kbox;
	t_quebox	*qbox;
	t_imgbox	*ibox;

	printf("key %d\n", key);
	kbox = (t_kernbox *)(((int **)param)[0]);
	qbox = (t_quebox *)(((int **)param)[1]);
	ibox = (t_imgbox *)(((int **)param)[2]);
	if (key == 53)
		clean_all(kbox, qbox, NULL);
	else if (key == 124)
	{
		kbox->i[1] += 1;
		reprint_all(kbox, qbox, ibox);
	}
	else if (key == 123)
	{
		kbox->i[1] -= 1;
		reprint_all(kbox, qbox, ibox);
	}
	return (0);
}

/*
 ** handle mouse events
 */

static int	click_mouse(int key, int x1, int x2, void *param)
{
	t_kernbox	*kbox;
	t_quebox	*qbox;
	t_imgbox	*ibox;

	printf("click %d  [%d, %d]\n", key, x1, x2);
	kbox = (t_kernbox *)(((int **)param)[0]);
	qbox = (t_quebox *)(((int **)param)[1]);
	ibox = (t_imgbox *)(((int **)param)[2]);
	if (key == 1 && x2 > 0)
	{
		ibox->mouse_left = 1;
		ibox->mouse_pos[0] = x1;
		ibox->mouse_pos[1] = x2;
	}
	if (key == 2 && x2 > 0)
	{
		ibox->mouse_right = 1;
		ibox->mouse_pos[0] = x1;
		ibox->mouse_pos[1] = x2;
	}
	if (key == 1 && x1 < 150 && x2 > 20 && x2 < 40)
	{
		init_mand(kbox, qbox, ibox);
		reprint_all(kbox, qbox, ibox);
	}
	if (key == 1 && x1 < 150 && x2 > 50 && x2 < 70)
	{
		init_jul(kbox, qbox, ibox);
		reprint_all(kbox, qbox, ibox);
	}
	if (key == 1 && x1 < 150 && x2 > 80 && x2 < 100)
	{
		init_ship(kbox, qbox, ibox);
		reprint_all(kbox, qbox, ibox);
	}
	else if (key == 5)
	{
		kbox->f[0] += (double)(x1 - HELP_W) * kbox->f[2] * (1 - SC_COEF);
		kbox->f[1] -= (double)x2 * kbox->f[2] * (1 - SC_COEF);
		kbox->f[2] *= SC_COEF;
		kbox->i[0] = depth(kbox->f[2]);
		if (kbox->i[0] > MAX_DEPTH)
			kbox->i[0] = (double)MAX_DEPTH;
		reprint_all(kbox, qbox, ibox);
	}
	else if (key == 4)
	{
		kbox->f[2] /= SC_COEF;
		kbox->f[0] += (double)(x1 - HELP_W) * kbox->f[2] * (SC_COEF - 1);
		kbox->f[1] -= (double)x2 * kbox->f[2] * (SC_COEF - 1);
		kbox->i[0] = depth(kbox->f[2]);
		if (kbox->i[0] > MAX_DEPTH)
			kbox->i[0] = (float)MAX_DEPTH;
		reprint_all(kbox, qbox, ibox);
	}
	return (0);
}

static int	release_mouse(int key, int x1, int x2, void *param)
{
	t_kernbox	*kbox;
	t_quebox	*qbox;
	t_imgbox	*ibox;

	kbox = (t_kernbox *)(((int **)param)[0]);
	qbox = (t_quebox *)(((int **)param)[1]);
	ibox = (t_imgbox *)(((int **)param)[2]);
	ibox->mouse_left = 0;
	ibox->mouse_right = 0;
	printf("release %d  [%d, %d]\n", key, x1, x2);
	return (0);
}

static int	move_mouse(int x1, int x2, void *param)
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
	printf("move [%d, %d]\n", x1, x2);
	return (0);
}

void	print_menu(void *mlx, void *wnd)
{
	mlx_string_put(mlx, wnd, 20, 20, CUR_COLOR, "Mandelbrot");
	mlx_string_put(mlx, wnd, 20, 50, HELP_COLOR, "Julia");
	mlx_string_put(mlx, wnd, 20, 80, HELP_COLOR, "Burning ship");
	mlx_string_put(mlx, wnd, 20, 220, HELP_COLOR, "scale by mouse wheel");
	mlx_string_put(mlx, wnd, 20, 250, HELP_COLOR, "scale (px):");
	mlx_string_put(mlx, wnd, 20, 320, HELP_COLOR, "depth's limit:");
	mlx_string_put(mlx, wnd, 200, 320, HELP_COLOR, ft_itoa(MAX_DEPTH));
	mlx_string_put(mlx, wnd, 20, 350, HELP_COLOR, "depth:");
	mlx_string_put(mlx, wnd, 20, 420, HELP_COLOR, "change color by < >");
	mlx_string_put(mlx, wnd, 20, 450, HELP_COLOR, "move by mouse left button");
}

int	main(int argc, char **argv)
{
	t_imgbox	ibox;
	t_quebox	qbox;
	t_kernbox	kbox;

	argv++; //
	if (argc != 1)
		clean_all(NULL, NULL, USAGE);
	if (!(ibox.mlx = mlx_init()))
		clean_all(NULL, NULL, ER_MLX);
	if (!(ibox.wnd = mlx_new_window(ibox.mlx, WND_W, WND_H, "Fractol")))
		clean_all(NULL, NULL, ER_WND);
	if (!(ibox.img = mlx_new_image(ibox.mlx, IMG_SIZE, IMG_SIZE)))
		clean_all(NULL, NULL, ER_IMG);
	if (!(ibox.eraser = mlx_new_image(ibox.mlx, HELP_W, STR_H)))
		clean_all(NULL, NULL, ER_IMG);
	ibox.mouse_left = 0;
	ibox.mouse_right = 0;
	init_queue(&qbox);
	print_menu(ibox.mlx, ibox.wnd);
	init_mand(&kbox, &qbox, &ibox);
	reprint_all(&kbox, &qbox, &ibox);
	mlx_hook(ibox.wnd, 2, 0, keyboard, 
			(int *[]){(int *)&kbox, (int *)&qbox, (int *)&ibox});
	mlx_hook(ibox.wnd, 4, 1, click_mouse, 
			(int *[]){(int *)&kbox, (int *)&qbox, (int *)&ibox});
	mlx_hook(ibox.wnd, 5, 1, release_mouse, 
			(int *[]){(int *)&kbox, (int *)&qbox, (int *)&ibox});
	mlx_hook(ibox.wnd, 6, 1, move_mouse, 
			(int *[]){(int *)&kbox, (int *)&qbox, (int *)&ibox});
	mlx_loop(ibox.mlx);
	return (0);
}
