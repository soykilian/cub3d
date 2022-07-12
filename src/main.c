#include <cub3d.h>
#include <mlx.h>

int	ft_hooks(int keycode, t_cub *vars)
{
	if (keycode == KEY_ESC)
	{
		//ft_free_all(vars);
		mlx_destroy_window(vars->mlx, vars->win);
		//free(vars.mlx);
		exit(0);
		return (1);
	}
	return (0);
}

int	main(void)
{
	t_cub	prog;

	prog.mlx = mlx_init();
	prog.win = mlx_new_window(prog.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
	mlx_hook(prog.win, 2, 1L << 0, ft_hooks, &prog);
	mlx_loop(prog.mlx);
	return (0);
}
