/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:55:54 by mbartole          #+#    #+#             */
/*   Updated: 2019/06/01 20:27:35 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# ifdef MAC
#  include <OpenCL/opencl.h>
# else
#  include <CL/cl.h>
# endif
# include "libft.h"
# include "mlx.h"
# include <math.h>
# include <fcntl.h>

typedef struct	s_imgbox
{
	void		*mlx;
	void		*wnd;
	void		*img;
	void		*eraser;
	char		mouse_left;
	char		mouse_right;
	int			mouse_pos[2];
}				t_imgbox;

typedef struct	s_quebox
{
	cl_context			ctx;
	cl_command_queue	queue;
	cl_program			prog;
}				t_quebox;

typedef struct	s_kernbox
{
	cl_kernel			kern;
	int					*map;
	cl_mem				map_buf;
	double				f[5];
	cl_mem				f_buf;
	int					i[2];
	cl_mem				i_buf;
}				t_kernbox;

# define IMG_SIZE 1000
# define WND_W 1400
# define WND_H 1000
# define HELP_W 400
# define STR_H 30
# define HELP_COLOR 0x00FF9100
# define CUR_COLOR 0x0043E8B1

# define SC_COEF 1.2
# define MAX_DEPTH 200000

# define PROGRAM_FILE "create_map.cl"

# define START_X -2.0
# define START_Y 2.0
# define START_SC 0.004

# define MAND "mandelbrot"

# define JUL "julia"
# define JUL_X 0.0
# define JUL_Y 0.0

# define SHIP "ship"

# define ER_MALLOC "cant malloc it!\n"
# define USAGE "usage: ./fractol [ '' | mandelbrot | julia | ship ]\n"

/*
** OpenCL enviroment
*/

void			init_queue(t_quebox *qbox);
void			init_kern(t_kernbox *kbox, char *kern_name, t_quebox *qbox,
					t_imgbox *ibox);

/*
** all mouse hooks
*/

void			hook_mouse(void *wnd, void *param);

/*
** utils
*/

void			clean_all(t_kernbox *kbox, t_quebox *qbox, char *msg);
void			init_fractol(t_kernbox *kbox, t_quebox *qbox, t_imgbox *ibox,
					char *name);
double			depth(double sc);
void			reprint_all(t_kernbox *kbox, t_quebox *qbox, t_imgbox *ibox);

#endif
