/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/03 13:54:08 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/04 14:48:35 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void		flag_toggle(t_vertex_pack *pack, const unsigned int flag)
{
	if (pack->flags_shader & flag)
		pack->flags_shader &= ~flag;
	else
		pack->flags_shader |= flag;
	glUniform1ui(pack->uniforms.flags, pack->flags_shader);
	ft_printf("new flags state: %b\n", pack->flags_shader);
}

void			light_move(int key, t_vertex_pack *pack, const float speed)
{
	t_v3f	*pos;

	pos = &pack->light.position;
	if (key == GLFW_KEY_KP_8)
		pos->y += speed;
	else if (key == GLFW_KEY_KP_5)
		pos->y += -speed;
	else if (key == GLFW_KEY_KP_4)
		pos->x += speed;
	else if (key == GLFW_KEY_KP_6)
		pos->x += -speed;
	else if (key == GLFW_KEY_KP_7)
		pos->z += -speed;
	else if (key == GLFW_KEY_KP_9)
		pos->z += speed;
	else
		return ;
	ft_printf("new light position: %.1f %.1f %.1f\n",
			(double)pos->x, (double)pos->y, (double)pos->z);
	glUniform3fv(pack->uniforms.light_pos, 1,
		(const GLfloat*)&pack->light.position);
}

