kernel void	mandelbrot(global int *map)
{
	int		index;
	float	sc;
	int		n;
	float2	c;
	float2	f;
	float2	prev;
	int		i;

	index = get_global_id(0);
	sc = 0.002;
	n = 100000;
	c = (float2)(-1.0 + (index % 1000) * sc, 1.0 - (index / 1000) * sc);
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
