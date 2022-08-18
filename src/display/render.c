/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsanchez <dsanchez@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 18:53:55 by dsanchez          #+#    #+#             */
/*   Updated: 2022/08/18 21:56:29 by dsanchez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

double	ft_wallx(t_render *params)
{
	double	wall_x;

	if (params->side == 0)
		wall_x = params->pos.y + params->perp_wall_dist * params->ray_dir.y;
	else
		wall_x = params->pos.x + params->perp_wall_dist * params->ray_dir.x;
	wall_x -= floor((wall_x));
	return (wall_x);
}

void	ft_draw_lines(t_cub *prog, t_render *params, int lineHeight, int x)
{
	int		color;
	double	step;
	double	tex_pos;
	int		tex_num;

	tex_num = ft_texnum(params);
	prog->tex_x = (int)(ft_wallx(params) * (double)prog->tex[tex_num].w);
	if (params->side == 0 && params->ray_dir.x > 0)
		prog->tex_x = prog->tex[tex_num].w - prog->tex_x - 1;
	if (params->side == 1 && params->ray_dir.y < 0)
		prog->tex_x = prog->tex[tex_num].w - prog->tex_x - 1;
	step = 1.0 * prog->tex[tex_num].h / lineHeight;
	tex_pos = (params->draw_start - HEIGHT / 2 + lineHeight / 2) * step;
	vertical_line(x, 0, params->draw_start, *prog);
	vertical_line(x, params->draw_end, HEIGHT, *prog);
	while (params->draw_start < params->draw_end)
	{
		prog->tex_y = (int)tex_pos & (prog->tex[tex_num].h - 1);
		tex_pos += step;
		color = get_pixel_color
			(&prog->tex[tex_num].img, prog->tex_x, prog->tex_y);
		if (params->side == 1)
			color = (color >> 1) & 8355711;
		my_mlx_pixel_put(&(prog->img), x, params->draw_start++, color);
	}
}

int	ft_loop_render(t_cub *prog, t_render *params, int x)
{
	int	line_height;

	printf("X: %f , Y: %f \n", params->pos.x, params->pos.y);
	printf("pos %c \n", prog->map[(int) params->pos.y][(int) params->pos.x]);
	ft_side_dist(params);
	if (!ft_dda(prog, params))
		return (0);
	line_height = ft_draw_start_end(params);
	ft_draw_lines(prog, params, line_height, x);
	ft_rotate(prog, params);
	ft_move(prog, params);
	return (1);
}

void	ft_init_render(t_cub *prog, t_render *params, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)WIDTH - 1;
	params->pos.x = prog->pos.x + 0.5;
	params->pos.y = prog->pos.y + 0.5;
	params->ray_dir.x = params->dir.x + params->plane.x * camera_x;
	params->ray_dir.y = params->dir.y + params->plane.y * camera_x;
	params->map_x = (int) params->pos.x;
	params->map_y = (int) params->pos.y;
	if (params->ray_dir.x == 0)
		params->delta_dist.x = 1e30;
	else
		params->delta_dist.x = ft_abs(1 / params->ray_dir.x);
	if (params->ray_dir.y == 0)
		params->delta_dist.y = 1e30;
	else
		params->delta_dist.y = ft_abs(1 / params->ray_dir.y);
}

int	ft_start(t_cub *prog)
{
	t_render	*params;
	int			x;

	params = prog->params;
	x = 0;
	ft_bzero(prog->img.addr, WIDTH * HEIGHT * (prog->img.bits_per_pixel / 8));
	while (x < WIDTH)
	{
		ft_init_render(prog, params, x);
		if (!ft_loop_render(prog, params, x))
			exit(0);
		x++;
	}
	mlx_put_image_to_window(prog->mlx, prog->win, prog->img.img, 0, 0);
	return (1);
}
