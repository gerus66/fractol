/*
** p.s0 - first_point.x
** p.s1	- first_point.y
** p.s2 - scale
*/

kernel void	mandelbrot(global int *map, global float3 *p)
{
	int		index;
	float	sc;
	int		n;
	float2	c;
	float2	f;
	float2	prev;
	int		i;
	float2	first;
	
	index = get_global_id(0);
	sc = p[0].s2;
	n = 60.0 * powr(0.004 / sc, 0.9);
	first = (float2)(p[0].s0, p[0].s1);
//	printf("[%.2f, %.2f] ", first.x, first.y);
	c = (float2)(first.x + (index % 1000) * sc, first.y - (index / 1000) * sc);
	f = (float2)(0, 0);
	i = -1;
	while (++i < n && f.x * f.x + f.y * f.y < 4)
	{
		prev = f;
		f.x = prev.x * prev.x - prev.y * prev.y + c.x;
		f.y = 2 * prev.x * prev.y + c.y;
	}
	map[index] = n / i;
//	printf("%d ", map[index]);
}
