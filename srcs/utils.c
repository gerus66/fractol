/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 01:06:06 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/27 07:17:00 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void		clean_all(t_kernbox *kbox, t_quebox *qbox, char *msg)
{
	ft_putstr(msg);
	if (kbox)
	{
		clReleaseKernel(kbox->kern);
		clReleaseMemObject(kbox->map_buf);
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

void		init_fractol(t_kernbox *kbox, t_quebox *qbox, t_imgbox *ibox,
		char *name)
{
	init_kern(kbox, name, qbox, ibox);
	kbox->f[0] = START_X;
	kbox->f[1] = START_Y;
	kbox->f[2] = START_SC;
	kbox->i[0] = depth(kbox->f[2]);
	kbox->i[1] = 1;
	if (!(ft_strcmp(name, JUL)))
	{
		kbox->f[3] = JUL_X;
		kbox->f[4] = JUL_Y;
	}
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 20);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 50);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 20, 80);
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 20,
			ft_strcmp(name, MAND) ? HELP_COLOR : CUR_COLOR, "Mandelbrot");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 50,
			ft_strcmp(name, JUL) ? HELP_COLOR : CUR_COLOR, "Julia");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 80,
			ft_strcmp(name, SHIP) ? HELP_COLOR : CUR_COLOR, "Burning ship");
}

/*
** another variant: 200.0 * log((sc - 250.0) / 100.0 + 2.0));
*/

double		depth(double sc)
{
	sc = 1 / sc;
	if (sc < 250)
		sc = 250;
	return (1000.0 * log10(sc - 248.0));
}

static void	print_image(t_imgbox *ibox, double sc, int iter)
{
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 150, 250);
	mlx_string_put(ibox->mlx, ibox->wnd, 150, 250, HELP_COLOR,
			ft_itoa((ssize_t)(1 / sc)));
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 200, 350);
	mlx_string_put(ibox->mlx, ibox->wnd, 200, 350, HELP_COLOR,
			ft_itoa(iter));
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->img, HELP_W, 0);
}

void		reprint_all(t_kernbox *kbox, t_quebox *qbox, t_imgbox *ibox)
{
	int		ret1;
	int		ret2;
	size_t	it;

	kbox->f_buf = clCreateBuffer(qbox->ctx, CL_MEM_READ_WRITE |
			CL_MEM_COPY_HOST_PTR, 5 * sizeof(double), kbox->f, &ret1);
	kbox->i_buf = clCreateBuffer(qbox->ctx, CL_MEM_READ_WRITE |
			CL_MEM_COPY_HOST_PTR, 2 * sizeof(int), kbox->i, &ret2);
	if (ret1 || ret2)
		clean_all(kbox, qbox, "cant create buffer\n");
	if (clSetKernelArg(kbox->kern, 1, sizeof(cl_mem), &(kbox->f_buf)) ||
			clSetKernelArg(kbox->kern, 2, sizeof(cl_mem), &(kbox->i_buf)))
		clean_all(kbox, qbox, "cant set arguments for kernel\n");
	it = IMG_SIZE * IMG_SIZE;
	if (clEnqueueNDRangeKernel(qbox->queue, kbox->kern, 1, 0, &it, 0, 0, 0, 0))
		clean_all(kbox, qbox, "cant enqueue the kernel\n");
	if (clEnqueueReadBuffer(qbox->queue, kbox->map_buf, CL_TRUE, 0,
				IMG_SIZE * IMG_SIZE * sizeof(int), kbox->map, 0, NULL, NULL))
		clean_all(kbox, qbox, "cant read from buffer\n");
	clReleaseMemObject(kbox->f_buf);
	clReleaseMemObject(kbox->i_buf);
	print_image(ibox, kbox->f[2], kbox->i[0]);
}
