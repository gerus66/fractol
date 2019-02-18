/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 15:47:17 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/18 14:42:11 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#define IBOX ((t_imgbox *)param)

void	clean_all(t_kernbox *kbox, t_quebox *qbox, char *msg)
{
	ft_putstr(msg);
	if (kbox)
	{
		clReleaseKernel(kbox->kern);
		clReleaseMemObject(kbox->map_buf);
		clReleaseMemObject(kbox->f_buf);
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

static int	keyboard(int key, void *param)
{
	param++;//
	if (key == 53)
		exit(0);//TODO
	//	clean_all((t_kernbox *)(param + 2), (t_quebox *)(param + 1), NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_imgbox	ibox;
	t_quebox	qbox;
	t_kernbox	kbox;
//	size_t		items;
	float		param[3];

	if (argc != 2 || ft_strcmp(argv[1], MAND))
		clean_all(NULL, NULL, USAGE);
	if (!(ibox.mlx = mlx_init()))
		clean_all(NULL, NULL, ER_MLX);
	if (!(ibox.wnd = mlx_new_window(ibox.mlx, WND_W, WND_H, argv[1])))
		clean_all(NULL, NULL, ER_WND);
	if (!(ibox.img = mlx_new_image(ibox.mlx, IMG_SIZE, IMG_SIZE)))
		clean_all(NULL, NULL, ER_IMG);
	init_queue(&qbox);
//	kbox.kern = clCreateKernel(qbox.prog, argv[1], &ret);
//	if (ret)
//		clean_all(&kbox, &qbox, "cant create kernel\n");
//	kbox.map = (int *)ft_memalloc(MAP_LEN * sizeof(int));
//	kbox.map_buf = clCreateBuffer(qbox.ctx, CL_MEM_READ_WRITE | 
//			CL_MEM_COPY_HOST_PTR, MAP_LEN * sizeof(int), kbox.map, &ret);
//	if (ret)
//		clean_all(kbox, qbox, "cant create buffer\n");
	init_kern(&kbox, argv[1], &qbox);
	param[0] = -2.0;
	param[1] = 1.0;
	param[2] = 0.002;
	evolve_kern(&kbox, &qbox, param);
//	items = MAP_LEN;
//	if (clEnqueueNDRangeKernel(qbox.queue, kbox.kern, 1, NULL, 
//			&items, NULL, 0, NULL, NULL))
//		clean_all(&kbox, &qbox, "cant enqueue the kernel\n");
//	clFinish(qbox.queue);
//	printf("finish!!!\n");
//	if (clEnqueueReadBuffer(qbox.queue, kbox.map_buf, CL_TRUE, 0,
//			MAP_LEN, kbox.map, 0, NULL, NULL))
//		clean_all(&kbox, &qbox, "cant read from buffer\n");
	paint_it(kbox.map, &ibox);
	//	create_map(&ibox);
	//	print_instructions(ibox.mlx, ibox.wnd);
	//	reprint_map(&ibox);
//	param = ft_memalloc(3 * sizeof(void));
//	param = (void *[3]){(void *)&ibox, (void *)&qbox, (void *)&kbox};
	mlx_hook(ibox.wnd, 2, 0, keyboard, &ibox);
	//	mlx_hook(ibox.wnd, 4, 1, mouse, &ibox);
	mlx_loop(ibox.mlx);
	return (0);
}
