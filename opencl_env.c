/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 20:42:37 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/17 21:02:08 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	init_prog(t_quebox *qbox, char *filename)
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
   		clean_all(NULL, qbox, ER_PROG);
	if ((ret = clBuildProgram(qbox->prog, 0, NULL, NULL, NULL, NULL)))
	{printf("build prog: %d\n", ret);
		clean_all(NULL, qbox, ER_PROG);}
}

void	init_queue(t_quebox *qbox)
{
	int	ret;
	cl_platform_id	platform;

	if (clGetPlatformIDs(1, &platform, NULL))
		clean_all(NULL, NULL, ER_PLT);
	if (clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &(qbox->dev), NULL))
		clean_all(NULL, NULL, ER_DEV);
	qbox->ctx = clCreateContext(NULL, 1, &(qbox->dev), NULL, NULL, &ret);
	if (ret)
		clean_all(NULL, NULL, ER_CONT);
	qbox->queue = clCreateCommandQueue(qbox->ctx, qbox->dev, 0, &ret);
	if (ret)
		clean_all(NULL, qbox, ER_QE);
	init_prog(qbox, PROGRAM_FILE);
}

void	init_kern(t_kernbox *kbox, char *kern_name, t_quebox *qbox)
{
//	cl_mem	map_buffer;
	int		ret;
//	int		n, x, y;

	kbox->kern = clCreateKernel(qbox->prog, kern_name, &ret);
	if (ret)
		clean_all(kbox, qbox, ER_KERN);
	kbox->map = (int *)ft_memalloc(IMG_SIZE * IMG_SIZE * sizeof(int));
	kbox->map_buf = clCreateBuffer(qbox->ctx, CL_MEM_READ_WRITE |
		CL_MEM_COPY_HOST_PTR, IMG_SIZE * IMG_SIZE * sizeof(int), kbox->map, &ret);
	if (ret)
		clean_all(kbox, qbox, "cant create buffer\n");
//	n = 100000;
//	x = -2.0;
//	y = 2.0;
	if (clSetKernelArg(kbox->kern, 0, sizeof(cl_mem), &(kbox->map_buf)))
//			clSetKernelArg(kbox->kern, 1, sizeof(int), &n) ||
//			clSetKernelArg(kbox->kern, 2, sizeof(float), &x) ||
//			clSetKernelArg(kbox->kern, 3, sizeof(float), &y))
		clean_all(kbox, qbox, "cant set arguments for kernel\n");
}
