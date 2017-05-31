/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/31 12:52:00 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/01 01:47:13 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#define AXIS_Y (t_v3d){0.0, 1.0, 0.0}
#define AXIS_X (t_v3d){1.0, 0.0, 0.0}

static void		mouse_look(t_vertex_pack *pack, const t_v2i vec)
{
	t_quaternion		*q;

	q = &pack->model_quat;
	if (vec.x != 0)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_Y, 0.002 * vec.x));
	if (vec.y != 0)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_X, 0.002 * vec.y));
	pack->model = geo_quat_tomatrix_offset(pack->model_quat, pack->model.w);
}

static double	clamp(const double x, const double min, const double max)
{
	if (x < min)
		return (min);
	return ((x > max) ? max : x);
}

static void		mouse_move(t_vertex_pack *pack, const t_v2i vec,
	const double speed)
{
	t_v4d		move;

	move = (t_v4d){
		.x = clamp((double)-vec.x * speed, -4.0, 4.0),
		.y = clamp((double)vec.y * speed, -4.0, 4.0),
		.z = 0.0,
		.w = 0.0
	};
	pack->camera.w = geo_addv4(move, pack->camera.w);
}

void			mouse_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	const t_v2i			pos = (t_v2i){(int)xpos, (int)ypos};
	t_v2i				*last;
	t_vertex_pack		*pack;

	(void)window;
	pack = get_pack(NULL);
	last = &pack->mouse_last;
	if ((pack->input & INPUT_RLAST) && (pack->input & INPUT_CLICK))
	{
		*last = pos;
		pack->input &= ~INPUT_RLAST;
	}
	if (pack->input & INPUT_LCLICK)
	{
		mouse_look(pack, (t_v2i){pos.x - last->x, pos.y - last->y});
		*last = pos;
	}
	else if (pack->input & INPUT_RCLICK)
		mouse_move(pack, (t_v2i){pos.x - last->x, pos.y - last->y}, 0.0005);
}

void			mouse_button_callback(GLFWwindow *window, int button,
		int action, int mods)
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
		pack->input |= INPUT_RLAST;
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			pack->input &= ~INPUT_LCLICK;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			pack->input &= ~INPUT_RCLICK;
	}
}
