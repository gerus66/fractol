/*
** pf.s0 - left_upper_corner.x
** pf.s1 - left_upper_corner.y
** pf.s2 - scale
** pi.s0 - n of iterations
** pi.s1 - parameter for color
*/

kernel void	mandelbrot(global int *map, global double3 *pf, global int2 *pi)
{
	int		index;
	int		i;
	double2	f;
	double2	prev;
	double4	col;
	
	index = get_global_id(0);
	col.zw = (double2){pf[0].s0 + (index % 1000) * pf[0].s2, 
			pf[0].s1 - (index / 1000) * pf[0].s2};
	f = col.zw;
	col.xy = (double2){f.x * f.x, f.y * f.y};
	prev = (double2){0.0, 0.0};
	i = -1;
	while (++i < pi[0].s0 && col.x + col.y < 4)
	{
		f.xy = (double2){col.x - col.y + col.z, 2 * f.x * f.y + col.w};
		if (prev.x == f.x && prev.y == f.y)
		{
			map[index] = 0;
			return ;
		}
		col.xy = (double2){f.x * f.x, f.y * f.y};
		prev = f;
	}
	if (i == pi[0].s0)
		map[index] = 0;
	else
	{
		col.w = (float)i - log2(log2(col.x + col.y)) + 4.0;
		col = 256.0 * (0.5 + 0.5 * cos((3.0 + col.w * 0.15 + 
						(double4){0.0, 0.0, 0.1 * pi[0].s1, 1.0})));
		map[index] = ((int)col.y << 16 | (int)col.z << 8 | (int)col.w);
	}
}

/*
** pf.s0 - left_upper_corner.x
** pf.s1 - left_upper_corner.y
** pf.s2 - scale
** pf.s3 - z0.x
** pf.s4 - z0.y
** pi.s0 - n of iterations
** pi.s1 - parameter for color
*/

kernel void	julia(global int *map, global double8 *pf, global int2 *pi)
{
	int		index;
	int		i;
	double2	f;
	double2	prev;
	double4	col;
	

	index = get_global_id(0);
	col.zw = (double2){pf[0].s3, pf[0].s4};
	f = (double2){pf[0].s0 + (index % 1000) * pf[0].s2, 
			pf[0].s1 - (index / 1000) * pf[0].s2};
	col.xy = (double2){f.x * f.x, f.y * f.y};
	prev = (double2){pf[0].s0, pf[0].s1};
	i = -1;
	while (++i < pi[0].s0 && col.x + col.y < 4)
	{
		f.xy = (double2){col.x - col.y + col.z, 2 * f.x * f.y + col.w};
		if (prev.x == f.x && prev.y == f.y)
		{
			map[index] = 0;
			return ;
		}
		col.xy = (double2){f.x * f.x, f.y * f.y};
		prev = f;
	}
	if (i == pi[0].s0)
		map[index] = 0;
	else
	{
		col.w = (float)i - log2(log2(col.x + col.y)) + 4.0;
		col = 256.0 * (0.5 + 0.5 * cos((3.0 + col.w * 0.15 + 
						(double4){0.0, 0.0, 0.5 + 0.1 * pi[0].s1, 1.0})));
		map[index] = ((int)col.y << 16 | (int)col.z << 8 | (int)col.w);
	}
}

/*
** pf.s0 - left_upper_corner.x
** pf.s1 - left_upper_corner.y
** pf.s2 - scale
** pi.s0 - n of iterations
** pi.s1 - parameter for color
*/

kernel void	ship(global int *map, global double3 *pf, global int2 *pi)
{
	int		index;
	int		i;
	double2	f;
	double2	prev;
	double4	col;
	
	index = get_global_id(0);
	col.zw = (double2){pf[0].s0 + (index % 1000) * pf[0].s2, 
			-pf[0].s1 + (index / 1000) * pf[0].s2};
	f = col.zw;
	col.xy = (double2){f.x * f.x, f.y * f.y};
	prev = (double2){0.0, 0.0};
	i = -1;
	while (++i < pi[0].s0 && col.x + col.y < 4)
	{
		f.xy = (double2){fabs(col.x - col.y + col.z), 
			fabs(2 * (f.x) * (f.y) + col.w)};
		if (prev.x == f.x && prev.y == f.y)
		{
			map[index] = 0;
			return ;
		}
		col.xy = (double2){f.x * f.x, f.y * f.y};
		prev = f;
	}
	if (i == pi[0].s0)
		map[index] = 0;
	else
	{
		col.w = (float)i - log2(log2(col.x + col.y)) + 4.0;
		col = 256.0 * (0.5 + 0.5 * cos((3.0 + col.w * 0.15 + 
						(double4){0.0, 0.0, 0.9 + 0.1 * pi[0].s1, 1.0})));
		map[index] = ((int)col.y << 16 | (int)col.z << 8 | (int)col.w);
	}
}
