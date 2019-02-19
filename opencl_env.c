/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 20:42:37 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/19 23:26:40 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	init_prog(t_quebox *qbox, char *filename)//TODO
{
	FILE *program_handle;
	char *program_buffer;
	size_t program_size;
	int ret;

	program_handle = fopen(filename, "r");
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);
	qbox->prog = clCreateProgramWithSource(qbox->ctx, 1, 
			(const char **)&program_buffer, &program_size, &ret);
	free(program_buffer);
	if(ret)
   		clean_all(NULL, qbox, "cant create program from file\n");
	if ((ret = clBuildProgram(qbox->prog, 0, NULL, NULL, NULL, NULL)))
	{
		size_t	log_size;
		clGetProgramBuildInfo(qbox->prog, qbox->dev, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char *log = (char *)ft_memalloc(log_size);
		clGetProgramBuildInfo(qbox->prog, qbox->dev, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("%s\n", log);
		clean_all(NULL, qbox, "cant build program\n");
	}
}

void	init_queue(t_quebox *qbox)
{
	int	ret;
	cl_platform_id	platform;

	if (clGetPlatformIDs(1, &platform, NULL))
		clean_all(NULL, NULL, "cant identify a platform\n");
	if (clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &(qbox->dev), NULL))
		clean_all(NULL, NULL, "cant access device\n");
	qbox->ctx = clCreateContext(NULL, 1, &(qbox->dev), NULL, NULL, &ret);
	if (ret)
		clean_all(NULL, NULL, "cant create context\n");
	qbox->queue = clCreateCommandQueue(qbox->ctx, qbox->dev, 0, &ret);
	if (ret)
		clean_all(NULL, qbox, "cant create command queue\n");
	init_prog(qbox, PROGRAM_FILE);
}

void	init_kern(t_kernbox *kbox, char *kern_name, t_quebox *qbox, t_imgbox *ibox)
{
	int		ret;
	int		bits;

	kbox->kern = clCreateKernel(qbox->prog, kern_name, &ret);
	if (ret)
		clean_all(kbox, qbox, "cant create kernel\n");
	kbox->map = (int *)mlx_get_data_addr(ibox->img, &bits, &ret, &ret);
	kbox->map_buf = clCreateBuffer(qbox->ctx, CL_MEM_READ_WRITE |
		CL_MEM_COPY_HOST_PTR, MAP_LEN * sizeof(int), kbox->map, &ret);
	if (ret)
		clean_all(kbox, qbox, "cant create buffer\n");
//	cl_uint a;
//	clGetKernelInfo(kbox->kern, CL_KERNEL_NUM_ARGS, 4, &a, NULL);
//	printf("kern args: %d\n", a);
	if (clSetKernelArg(kbox->kern, 0, sizeof(cl_mem), &(kbox->map_buf)))
		clean_all(kbox, qbox, "cant set arguments for kernel\n");
}

void	reprint_all(t_kernbox *kbox, t_quebox *qbox, t_imgbox *ibox)
{
	int		ret;
	size_t	items;

	kbox->f_buf = clCreateBuffer(qbox->ctx, CL_MEM_READ_WRITE |
			CL_MEM_COPY_HOST_PTR, 4 * sizeof(double), kbox->f, &ret);
	if (ret)
		clean_all(kbox, qbox, "cant create buffer\n");
	if (clSetKernelArg(kbox->kern, 1, sizeof(cl_mem), &(kbox->f_buf)))
		clean_all(kbox, qbox, "cant set arguments for kernel\n");
	items = MAP_LEN;
	if (clEnqueueNDRangeKernel(qbox->queue, kbox->kern, 1, NULL,
				&items, NULL, 0, NULL, NULL))
		clean_all(kbox, qbox, "cant enqueue the kernel\n");
	if (clEnqueueReadBuffer(qbox->queue, kbox->map_buf, CL_TRUE, 0,
				MAP_LEN * sizeof(int), kbox->map, 0, NULL, NULL))
		clean_all(kbox, qbox, "cant read from buffer\n");
	clReleaseMemObject(kbox->f_buf);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->img, WND_W - IMG_SIZE,
			WND_H - IMG_SIZE);
	mlx_put_image_to_window(ibox->mlx, ibox->wnd, ibox->eraser, 0, 0);
	print_menu(ibox->mlx, ibox->wnd);
	mlx_string_put(ibox->mlx, ibox->wnd, 150, 150, HELP_COLOR, 
			ft_itoa((ssize_t)(1 / kbox->f[2])));
	mlx_string_put(ibox->mlx, ibox->wnd, 200, 250, HELP_COLOR, 
			ft_itoa(kbox->f[3]));
}
