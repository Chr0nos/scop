/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/31 12:52:00 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 04:25:28 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static void		mouse_look(struct s_transform *transform, const t_v2i vec)
{
	t_quaternion		*q;

	q = &transform->q;
	if (vec.x != 0)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_Y, 0.002 * vec.x));
	if (vec.y != 0)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_X, 0.002 * vec.y));
	transform->matrix = geo_quat_tomatrix_offset(*q, transform->matrix.w);
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
		.x = clamp((double)-vec.x * speed, -1.0, 1.0),
		.y = clamp((double)vec.y * speed, -1.0, 1.0),
		.z = 0.0,
		.w = 0.0
	};
	pack->camera.w = geo_addv4(move, pack->camera.w);
}

void			mouse_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
	const t_v2i			pos = (t_v2i){(int)xpos, (int)ypos};
	t_v2i				*last;
	t_vertex_pack		*pack;

	(void)window;
	pack = &g_pack;
	last = &pack->mouse_last;
	if (pack->input & INPUT_CLICK)
	{
		if (pack->input & INPUT_RLAST)
			*last = pos;
		pack->input &= ~INPUT_RLAST;
	}
	if (pack->input & INPUT_LCLICK)
		mouse_look(&pack->object.transform,
			(t_v2i){pos.x - last->x, pos.y - last->y});
	else if (pack->input & INPUT_RCLICK)
		mouse_move(pack, (t_v2i){pos.x - last->x, pos.y - last->y}, 0.01);
	if (pack->input & INPUT_CLICK)
		*last = pos;
}

void			mouse_button_callback(GLFWwindow *window, int button,
		int action, int mods)
{
	t_vertex_pack	*pack;

	(void)window;
	(void)mods;
	pack = &g_pack;
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
