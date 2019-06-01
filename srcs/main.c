/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 15:47:17 by mbartole          #+#    #+#             */
/*   Updated: 2019/03/31 17:08:39 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/*
** handle keyboard events
*/

static int		keyboard(int key, void *param)
{
	t_kernbox	*kbox;
	t_quebox	*qbox;
	t_imgbox	*ibox;

	kbox = (t_kernbox *)(((int **)param)[0]);
	qbox = (t_quebox *)(((int **)param)[1]);
	ibox = (t_imgbox *)(((int **)param)[2]);
	if (key == 53)
		clean_all(kbox, qbox, NULL);
	else if (key == 124)
		kbox->i[1] += 1;
	else if (key == 123)
		kbox->i[1] -= 1;
	if (key == 123 || key == 124)
		reprint_all(kbox, qbox, ibox);
	return (0);
}

/*
** close window on red "X"
*/

static int		xclose(void *param)
{
	clean_all((t_kernbox *)(((int **)param)[0]),
			(t_quebox *)(((int **)param)[1]), NULL);
	return (0);
}

/*
** initialize all MLX atributes and mouse flags,
** print left menu
*/

static void		init_image(t_imgbox *ibox)
{
	if (!(ibox->mlx = mlx_init()))
		clean_all(NULL, NULL, "cant initialize mlx\n");
	if (!(ibox->wnd = mlx_new_window(ibox->mlx, WND_W, WND_H, "Fractol")))
		clean_all(NULL, NULL, "cant create new window\n");
	if (!(ibox->img = mlx_new_image(ibox->mlx, IMG_SIZE, IMG_SIZE)))
		clean_all(NULL, NULL, "cant create new image\n");
	if (!(ibox->eraser = mlx_new_image(ibox->mlx, HELP_W, STR_H)))
		clean_all(NULL, NULL, "cant create new image\n");
	ibox->mouse_left = 0;
	ibox->mouse_right = 0;
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 20, HELP_COLOR, "Mandelbrot");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 50, HELP_COLOR, "Julia");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 80, HELP_COLOR, "Burning ship");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 220, HELP_COLOR,
			"scale by mouse wheel");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 250, HELP_COLOR, "scale (px):");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 320, HELP_COLOR, "depth's limit:");
	mlx_string_put(ibox->mlx, ibox->wnd, 200, 320, HELP_COLOR,
			ft_itoa(MAX_DEPTH));
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 350, HELP_COLOR, "depth:");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 420, HELP_COLOR,
			"change color by < >");
	mlx_string_put(ibox->mlx, ibox->wnd, 20, 450, HELP_COLOR,
			"move by mouse left button");
}

int				main(int argc, char **argv)
{
	t_imgbox	ibox;
	t_quebox	qbox;
	t_kernbox	kbox;
	char		*name;

	name = (argc == 2) ? argv[1] : MAND;
	if (argc > 2 || (ft_strcmp(name, MAND) && ft_strcmp(name, JUL)
				&& ft_strcmp(name, SHIP)))
		clean_all(NULL, NULL, USAGE);
	init_image(&ibox);
	init_queue(&qbox);
	init_fractol(&kbox, &qbox, &ibox, name);
	reprint_all(&kbox, &qbox, &ibox);
	mlx_hook(ibox.wnd, 17, 0, xclose, (int *[]){(int *)&kbox, (int *)&qbox});
	mlx_hook(ibox.wnd, 2, 0, keyboard,
			(int *[]){(int *)&kbox, (int *)&qbox, (int *)&ibox});
	hook_mouse(ibox.wnd, (int *[]){(int *)&kbox, (int *)&qbox, (int *)&ibox});
	mlx_loop(ibox.mlx);
	return (0);
}
