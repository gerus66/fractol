/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:55:54 by mbartole          #+#    #+#             */
/*   Updated: 2019/02/26 09:18:45 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "OpenCL/opencl.h"
# include "libft/libft.h"
# include "mlx.h"
# include <math.h>

#include <stdio.h>

/*typedef union	u_color
{
	int				cl;
	unsigned char	cl4[4];
}				t_color;
*/

typedef struct	s_imgbox
{
	void		*mlx;
	void		*wnd;
	void		*img;
	void		*eraser;
	char		mouse_left;
	char		mouse_right;
	int			mouse_pos[2];
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
	int					*map;
	cl_mem				map_buf;
	double				f[5];
	cl_mem				f_buf;
	int					i[2];
	cl_mem				i_buf;
}				t_kernbox;

# define IMG_SIZE 1000
# define MAP_LEN 1000000
# define WND_W 1400
# define WND_H 1000
# define HELP_W 400
# define STR_H 30
# define HELP_COLOR 0x00FF9100
# define CUR_COLOR 0x0043E8B1

# define SC_COEF 1.2
# define MOVE_PX 50;
# define MAX_DEPTH 200000

# define PROGRAM_FILE "create_map.cl"

# define MAND "mandelbrot"
# define MAND_START_X -2.0
# define MAND_START_Y 2.0
# define MAND_START_SC 0.004

# define JUL "julia"
# define JUL_START_X 0.0
# define JUL_START_Y 0.0

# define SHIP "ship"

# define ER_MAL "cant malloc it!\n"
# define ER_MLX "cannot initialize mlx\n"
# define ER_WND "cannot create new window\n"
# define ER_IMG "cannot create new image\n"
# define USAGE "Usage will be here\n"

void			init_queue(t_quebox *qbox);
void			init_kern(t_kernbox *kbox, char *kern_name, t_quebox *qbox, t_imgbox *ibox);
void			evolve_kern(t_kernbox *kbox, t_quebox *qbox);
void			clean_all(t_kernbox *kbox, t_quebox *qbox, char *msg);
void			reprint_all(t_kernbox *kbox, t_quebox *qbox, t_imgbox *ibox);
void			print_menu(void *mlx, void *wnd);

#endif
