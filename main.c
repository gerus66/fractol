/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 15:47:17 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/19 16:44:32 by mbartole         ###   ########.fr       */
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

/*
** handle keyboard events
*/

static int	keyboard(int key, void *param)
{
	if (key == 53)
		clean_all((t_kernbox *)(((int **)param)[0]),
			(t_quebox *)(((int **)param)[1]), NULL);
	return (0);
}

/*
** handle mouse events
*/

static int	mouse(int key, int x1, int x2, void *param)
{
	t_kernbox	*kbox;
	t_quebox	*qbox;
	t_imgbox	*ibox;
	float		tmp;

	kbox = (t_kernbox *)(((int **)param)[0]);
	qbox = (t_quebox *)(((int **)param)[1]);
	ibox = (t_imgbox *)(((int **)param)[2]);
	if (key == 5)
	{
	//	tmp = kbox->f[2];
		kbox->f[0] += (float)(x1 - HELP_W) * kbox->f[2] * (1 - SC_COEF);
		kbox->f[1] -= (float)x2 * kbox->f[2] * (1 - SC_COEF);
		kbox->f[2] *= SC_COEF;
	reprint_all(kbox, qbox, ibox);
	}
	else if (key == 4)
	{
		tmp = kbox->f[2];
		kbox->f[2] /= SC_COEF;
		kbox->f[0] += (float)(x1 - HELP_W) * kbox->f[2] * (SC_COEF - 1);
		kbox->f[1] -= (float)x2 * kbox->f[2] * (SC_COEF - 1);
	printf("1/sc = %d n = %d\n", (int)(1 / kbox->f[2]), 
			(int)(50.0 * pow(0.004 / kbox->f[2], 0.9)));
	reprint_all(kbox, qbox, ibox);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_imgbox	ibox;
	t_quebox	qbox;
	t_kernbox	kbox;

	if (argc != 2 || ft_strcmp(argv[1], MAND))
		clean_all(NULL, NULL, USAGE);
	if (!(ibox.mlx = mlx_init()))
		clean_all(NULL, NULL, ER_MLX);
	if (!(ibox.wnd = mlx_new_window(ibox.mlx, WND_W, WND_H, argv[1])))
		clean_all(NULL, NULL, ER_WND);
	if (!(ibox.img = mlx_new_image(ibox.mlx, IMG_SIZE, IMG_SIZE)))
		clean_all(NULL, NULL, ER_IMG);
	init_queue(&qbox);
	init_kern(&kbox, argv[1], &qbox);
	kbox.f[0] = MAND_START_X;
	kbox.f[1] = MAND_START_Y;
	kbox.f[2] = MAND_START_SC;
	//	print_instructions(ibox.mlx, ibox.wnd);
	reprint_all(&kbox, &qbox, &ibox);
	mlx_hook(ibox.wnd, 2, 0, keyboard, (int *[]){(int *)&kbox, (int *)&qbox});
	mlx_hook(ibox.wnd, 4, 1, mouse, 
		(int *[]){(int *)&kbox, (int *)&qbox, (int *)&ibox});
	mlx_loop(ibox.mlx);
	return (0);
}
