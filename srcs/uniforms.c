/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniforms.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/28 11:24:17 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/31 14:14:57 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static void			send_uniforms_tex(GLint uniform, GLuint id, GLint value)
{
	ft_printf("-> %d %u %d\n", uniform, id, value);
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, id);
	glUniform1i(uniform, value);
}


void				send_uniforms(GLFWwindow *window, t_vertex_pack *pack)
{
	t_uniforms		*u;
	t_m4f			proj;

	u = &pack->uniforms;
	pack->light = (t_light){
		.position = (t_v3f){0.0f, 0.0f, 25.0f},
		.color = (t_v4f){1.0f, 1.0f, 1.0f, 1.0f}
	};
	proj = geo_mk4_tof(get_projection(window, DISPLAY_FOV, 1.0, 100.0));
	u->proj = glGetUniformLocation(pack->program, "projection");
	u->model_view = glGetUniformLocation(pack->program, "model");
	u->camera = glGetUniformLocation(pack->program, "view");
	u->light_pos = glGetUniformLocation(pack->program, "light.position");
	u->light_color = glGetUniformLocation(pack->program, "light.color");
	u->texture_switch = glGetUniformLocation(pack->program, "tex_switch");
	u->texture_switch_val = 0.0f;
	u->texture_switch_mode = FLAG_SW_NONE;
	glUniformMatrix4fv(u->proj, 1, GL_FALSE, (const GLfloat *)&proj);
	glUniform1f(u->texture_switch, u->texture_switch_val);
	glUniform3fv(u->light_pos, 1, (const GLfloat *)&pack->light.position);
	glUniform4fv(u->light_color, 1, (const GLfloat *)&pack->light.color);
	send_uniforms_tex(pack->texture_id, pack->texture, 1);
	if (pack->normal_map)
		send_uniforms_tex(pack->normal_map_id, pack->normal_map, 2);
	glUniformMatrix4fv(u->camera, 1, GL_FALSE, (const GLfloat *)&pack->camera);
}
