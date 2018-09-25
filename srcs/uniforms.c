/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniforms.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/28 11:24:17 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 00:09:15 by snicolet         ###   ########.fr       */
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

static void			set_uniforms_ids(t_vertex_pack *pack, t_uniforms *u)
{
	u->proj = glGetUniformLocation(pack->program, "projection");
	u->model_view = glGetUniformLocation(pack->program, "model");
	u->camera = glGetUniformLocation(pack->program, "view");
	u->light_pos = glGetUniformLocation(pack->program, "light.position");
	u->light_color = glGetUniformLocation(pack->program, "light.color");
	u->texture_switch = glGetUniformLocation(pack->program, "tex_switch");
	u->flags = glGetUniformLocation(pack->program, "flags");
}

void				send_uniforms(GLFWwindow *window, t_vertex_pack *pack)
{
	t_uniforms		*u;
	t_m4f			proj;

	u = &pack->uniforms;
	pack->light = (t_light){
		.position = (t_v3f){0.0f, 0.0f, 9.0f},
		.color = (t_v4f){1.0f, 1.0f, 1.0f, 1.0f}
	};
	proj = geo_mk4_tof(get_projection(window, pack->fov, 1.0, 100.0));
	set_uniforms_ids(pack, u);
	u->texture_switch_val = 0.0f;
	u->texture_switch_mode = FLAG_SW_NONE;
	glUniformMatrix4fv(u->proj, 1, GL_FALSE, (const GLfloat *)&proj);
	glUniform1f(u->texture_switch, u->texture_switch_val);
	glUniform3fv(u->light_pos, 1, (const GLfloat *)&pack->light.position);
	glUniform4fv(u->light_color, 1, (const GLfloat *)&pack->light.color);
	send_uniforms_tex(pack->tex_diffuse.opengl_id, pack->tex_diffuse.id, 1);
	if (pack->tex_normal_map.id)
		send_uniforms_tex(pack->tex_normal_map.opengl_id, pack->tex_normal_map.id, 2);
	glUniformMatrix4fv(u->camera, 1, GL_FALSE, (const GLfloat *)&pack->camera);
	glUniform1ui(u->flags, pack->flags_shader);
	ft_printf("shader flags: %b\n", pack->flags_shader);
}
