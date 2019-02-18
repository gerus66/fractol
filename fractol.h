/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:55:54 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/18 11:51:06 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "OpenCL/opencl.h"
# include "libft/libft.h"
# include "mlx.h"
# include <math.h>

#include <stdio.h>

typedef union	u_color
{
	int				cl;
	unsigned char	cl4[4];
}				t_color;


typedef struct	s_imgbox
{
	void		*mlx;
	void		*wnd;
	void		*img;
	//	double		rot[16];
	//	double		mov[16];
	//	double		scl[16];
	//	char		p_flag;
}				t_imgbox;

typedef struct	s_quebox
{
	cl_device_id		dev;
	cl_context			ctx;
	cl_command_queue	queue;
	cl_program			prog;
}				t_quebox;

typedef struct	s_kernbox
{
	cl_kernel			kern;
	cl_mem				map_buf;
	int					*map;
}				t_kernbox;

# define IMG_SIZE 1000
# define WND_W 1300
# define WND_H 1000
# define PROGRAM_FILE "create_map.cl"
# define MAND "mandelbrot"
# define ER_MAL "cant malloc it!\n"
# define ER_MLX "cannot initialize mlx\n"
# define ER_WND "cannot create new window\n"
# define ER_IMG "cannot create new image\n"
# define ER_PLT "cannot identify a platform\n"
# define ER_DEV "cannot access device\n"
# define ER_CONT "cannot create context\n"
# define ER_QE "cannot create command queue\n"
# define ER_PROG "cannot create program from file\n"
# define ER_KERN "cannot create kernel\n"
# define USAGE "Usage will be here\n"

void			init_queue(t_quebox *qbox);
void			init_kern(t_kernbox *kbox, char *kern_name, t_quebox *qbox);
void			clean_all(t_kernbox *kbox, t_quebox *qbox, char *msg);
void			paint_it(int *map, t_imgbox *ibox);

#endif
