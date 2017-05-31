/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/31 12:52:00 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/31 17:08:35 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void		mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	const t_v2i			pos = (t_v2i){(int)xpos, (int)ypos};
	static t_v2i		last = (t_v2i){0, 0};
	t_v4d				offset;
	t_v2i				vec;
	t_vertex_pack		*pack;

	(void)window;
	pack = get_pack(NULL);
	if (pack->input & INPUT_LCLICK)
	{
		vec = (t_v2i){pos.x - last.x, pos.y - last.y};
		offset = pack->camera.w;
		if (vec.x != 0)
			pack->camera_quat = geo_quat_mult(pack->camera_quat, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, 0.002 * vec.x));
		if (vec.y != 0)
			pack->camera_quat = geo_quat_mult(pack->camera_quat, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, 0.002 * vec.y));
		pack->camera.w = offset;
		pack->camera = geo_quat_tomatrix(pack->camera_quat);
		ft_printf("mouse move: %d %d -- %d %d\n", pos.x, pos.y, vec.x, vec.y);
		last = pos;
	}
}

void		mouse_button_callback(GLFWwindow *window, int button, int action,
		int mods)
{
	t_vertex_pack	*pack;

	(void)window;
	(void)mods;
	pack = get_pack(NULL);
	if (action == 1)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			pack->input |= INPUT_LCLICK;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			pack->input |= INPUT_RCLICK;
	}
	else
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			pack->input &= ~INPUT_LCLICK;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			pack->input &= ~INPUT_RCLICK;
	}
}
