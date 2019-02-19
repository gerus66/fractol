/*
** p.s0 - first_point.x
** p.s1	- first_point.y
** p.s2 - scale
** p.s3 - n of iterations
*/

kernel void	mandelbrot(global int *map, global double4 *p)
{
	int		index;
	double	sc;
	size_t		n;
	double2	c;
	double2	f;
	double2	prev;
	size_t		i;
	double2	first;
	size_t	tmp;
	
	index = get_global_id(0);
	sc = p[0].s2;
	n = (int)p[0].s3;
	first = (double2)(p[0].s0, p[0].s1);
	c = (double2)(first.x + (index % 1000) * sc, first.y - (index / 1000) * sc);
	f = (double2)(0, 0);
	i = -1;
	while (++i < n && f.x * f.x + f.y * f.y < 4)
	{
		prev = f;
		f.x = prev.x * prev.x - prev.y * prev.y + c.x;
		f.y = 2 * prev.x * prev.y + c.y;
	}
//	tmp = n / i;
	if (i == n)
		map[index] = 0;
	else
	{
		tmp = 0x00FF0000 * n / i;
		map[index] = tmp % 0x00FFFFFF;
//		map[index] = n / i < 0x00000101 ? tmp : 0;
	}
//	map[index] = tmp < 0x0000FFFF ? tmp : 0x00FD6969;
}
