/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/31 12:52:00 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/31 23:13:31 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#define AXIS_Y (t_v3d){0.0, 1.0, 0.0}
#define AXIS_X (t_v3d){1.0, 0.0, 0.0}

static void	mouse_look(t_vertex_pack *pack, const t_v2i vec)
{
	t_quaternion		*q;

	q = &pack->model_quat;
	if (vec.x != 0)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_Y, 0.002 * vec.x));
	if (vec.y != 0)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_X, 0.002 * vec.y));
	pack->model = geo_quat_tomatrix_offset(pack->model_quat, pack->model.w);
}

void		mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	const t_v2i			pos = (t_v2i){(int)xpos, (int)ypos};
	t_v2i				*last;
	t_v2i				vec;
	t_vertex_pack		*pack;

	(void)window;
	pack = get_pack(NULL);
	last = &pack->mouse_last;
	if (pack->input & INPUT_LCLICK)
	{
		if (pack->input & INPUT_RLAST)
		{
			*last = pos;
			pack->input &= ~INPUT_RLAST;
		}
		vec = (t_v2i){pos.x - last->x, pos.y - last->y};
		mouse_look(pack, vec);
		*last = pos;
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
		{
			pack->input &= ~INPUT_LCLICK;
			pack->input |= INPUT_RLAST;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			pack->input &= ~INPUT_RCLICK;
	}
}
