/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/27 04:04:14 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "geo.h"
#include "quaternion.h"

static void			matrix_keyboard(GLFWwindow *window, t_quaternion *q,
	t_v4d *cam, const double speed)
{
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		cam->z -= speed;
	else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		cam->z += speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam->x -= speed;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam->x += speed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam->y += speed;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam->y -= speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, 0.02));
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, -0.02));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, -0.02));
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, 0.02));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 0.0, 1.0}, -0.02));
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 0.0, 1.0}, 0.02));
}

t_m4				make_matrix(GLFWwindow *window)
{
	static t_v4d			camera = (t_v4d){0.0, 0.0, 5.0, 1.0};
	t_m4					m;
	static t_quaternion		q = (t_quaternion){1.0, 0.0, 0.0, 0.0};

	matrix_keyboard(window, &q, &camera,
		glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 0.5 : 0.1);
	m = geo_quat_tomatrix(q);
	m.w = camera;
	return (m);
}

t_m4				get_projection(GLFWwindow *window, double fov, double far,
	double near)
{
	double			ratio;
	double			height;
	double			width;
	t_m4			proj;
	t_v2i			geo;

	glfwGetWindowSize(window, &geo.x, &geo.y);
	ft_printf("window resolution: %dx%d\n", geo.x, geo.y);
	ratio = (double)geo.x / (double)geo.y;
	height = near * (tan(fov * 0.5 * (M_PI / 180)));
	width = height * ratio;
	proj = geo_mk4_projection(
		(t_proj){-width, width, -height, height, near, far});
	return (proj);
}

static void			send_uniforms_tex(GLint uniform, GLuint id, GLint value)
{
	ft_printf("-> %d %u %d\n", uniform, id, value);
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, id);
	glUniform1i(uniform, value);
}

static void			send_uniforms(GLFWwindow *window, t_vertex_pack *pack)
{
	t_uniforms		*u;
	t_m4f			proj;

	u = &pack->uniforms;
	pack->light = (t_light){
		.position = (t_v3f){0.0f, 0.0f, -5.0f},
		.color = (t_v4f){1.0f, 1.0f, 1.0f, 1.0f}
	};
	proj = geo_mk4_tof(get_projection(window, DISPLAY_FOV, 1.0, 100.0));
	u->proj = glGetUniformLocation(pack->program, "projection");
	u->model_view = glGetUniformLocation(pack->program, "model");
	u->light_pos = glGetUniformLocation(pack->program, "light.position");
	u->light_color = glGetUniformLocation(pack->program, "light.color");
	u->texture_switch = glGetUniformLocation(pack->program, "tex_switch");
	u->texture_switch_val = 0.0f;
	u->texture_switch_mode = FLAG_SW_NONE;
	glUniformMatrix4fv(u->proj, 1, GL_FALSE, (const GLfloat *)&proj);
	glUniform1f(u->texture_switch, u->texture_switch_val);
	glUniform3fv(u->light_pos, 1, (const GLfloat *)&pack->light.position);
	glUniform4fv(u->light_color, 1, (const GLfloat *)&pack->light.color);
	send_uniforms_tex(pack->texture_id, pack->texture, 0);
	if (pack->normal_map)
		send_uniforms_tex(pack->normal_map_id, pack->normal_map, 1);
}

/*
** this function manage the folowings keys: Z X C
** press Z to incrase texture intenssity
** press X to decrase texture intenssity
** press C to switch texture intenssity using a smooth fade effect
** the value of u->texture_switch_val will be sent to the graphic card
** it will be already clamped
** the default state of u->texture_switch_mode is FLAG_SW_NONE
*/

static void			event_texture_mode(GLFWwindow *window, t_uniforms *u)
{
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		u->texture_switch_val -= 0.05f;
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		u->texture_switch_val += 0.05f;
	if (u->texture_switch_mode != FLAG_SW_NONE)
	{
		u->texture_switch_val +=
			(u->texture_switch_mode & FLAG_SW_IN) ? -0.015f : 0.015f;
		if ((u->texture_switch_val <= 0.0f) || (u->texture_switch_val >= 1.0f))
			u->texture_switch_mode = FLAG_SW_NONE;
	}
	u->texture_switch_val = geo_clamp(u->texture_switch_val, 0.0f, 1.0f);
	glUniform1f(u->texture_switch, u->texture_switch_val);
}

/*
** main display_loop
*/

int					display_loop(GLFWwindow *window, t_vertex_pack *pack)
{
	const int		faces_total = (int)(pack->stats.faces * 3);
	t_m4f			modelview;

	glUseProgram(pack->program);
	send_uniforms(window, pack);
	while ((!glfwWindowShouldClose(window)) && (!keyboard(window)))
	{
		event_texture_mode(window, &pack->uniforms);
		modelview = geo_mk4_tof(make_matrix(window));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(pack->uniforms.model_view, 1, GL_FALSE,
			(const GLfloat *)&modelview);
		glBindVertexArray(pack->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pack->indices);
		glDrawElements(GL_TRIANGLES, faces_total, GL_UNSIGNED_INT, NULL);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return (0);
}
