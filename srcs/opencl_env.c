/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 20:42:37 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/27 07:12:56 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static char	*program_buffer(char *name)
{
	int		fd;
	char	*buf;
	int		r;
	size_t	l;

	if ((fd = open(name, O_RDONLY)) == -1)
		clean_all(NULL, NULL, "cant open program file\n");
	l = 0;
	while ((r = get_next_line(fd, &buf)))
	{
		if (r == -1)
			clean_all(NULL, NULL, "cant read from program file\n");
		l += ft_strlen(buf) + 1;
		free(buf);
	}
	close(fd);
	if (!(buf = (char*)malloc(l + 1)))
		clean_all(NULL, NULL, ER_MALLOC);
	if ((fd = open(name, O_RDONLY)) == -1 || (read(fd, buf, l)) == -1)
		clean_all(NULL, NULL, "cant read from program file\n");
	buf[l] = '\0';
	close(fd);
	return (buf);
}

static void	cook_program(t_quebox *qbox, cl_device_id *dev, char *buf)
{
	size_t	len;
	int		ret;

	len = ft_strlen(buf);
	qbox->prog = clCreateProgramWithSource(qbox->ctx, 1,
			(const char **)&buf, &len, &ret);
	free(buf);
	if (ret)
	{
		clReleaseContext(qbox->ctx);
		clReleaseCommandQueue(qbox->queue);
		clean_all(NULL, NULL, "cant create program from file\n");
	}
	if ((ret = clBuildProgram(qbox->prog, 0, NULL, NULL, NULL, NULL)))
	{
		clGetProgramBuildInfo(qbox->prog, *dev,
				CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
		if (!(buf = (char *)ft_memalloc(len)))
			clean_all(NULL, qbox, ER_MALLOC);
		clGetProgramBuildInfo(qbox->prog, *dev,
				CL_PROGRAM_BUILD_LOG, len, buf, NULL);
		write(1, buf, len);
		free(buf);
		clean_all(NULL, qbox, "cant build program\n");
	}
}

void		init_queue(t_quebox *qbox)
{
	int				ret;
	cl_platform_id	platform;
	cl_device_id	dev;
	char			*buf;

	buf = program_buffer(PROGRAM_FILE);
	if (clGetPlatformIDs(1, &platform, NULL))
		clean_all(NULL, NULL, "cant identify a platform\n");
	if (clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL))
		clean_all(NULL, NULL, "cant access device\n");
	qbox->ctx = clCreateContext(NULL, 1, &dev, NULL, NULL, &ret);
	if (ret)
	{
		free(buf);
		clean_all(NULL, NULL, "cant create context\n");
	}
	qbox->queue = clCreateCommandQueue(qbox->ctx, dev, 0, &ret);
	if (ret)
	{
		free(buf);
		clReleaseContext(qbox->ctx);
		clean_all(NULL, NULL, "cant create command queue\n");
	}
	cook_program(qbox, &dev, buf);
}

void		init_kern(t_kernbox *kbox, char *kname, t_quebox *qbox,
		t_imgbox *ibox)
{
	int		ret;
	int		bits;

	kbox->kern = clCreateKernel(qbox->prog, kname, &ret);
	if (ret)
		clean_all(kbox, qbox, "cant create kernel\n");
	kbox->map = (int *)mlx_get_data_addr(ibox->img, &bits, &ret, &ret);
	kbox->map_buf = clCreateBuffer(qbox->ctx, CL_MEM_READ_WRITE |
			CL_MEM_COPY_HOST_PTR, IMG_SIZE * IMG_SIZE * sizeof(int),
			kbox->map, &ret);
	if (ret)
		clean_all(kbox, qbox, "cant create buffer\n");
	if (clSetKernelArg(kbox->kern, 0, sizeof(cl_mem), &(kbox->map_buf)))
		clean_all(kbox, qbox, "cant set arguments for kernel\n");
}
