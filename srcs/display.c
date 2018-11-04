/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 04:23:20 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "geo.h"

t_m4				get_projection(GLFWwindow *window, double fov, double far,
	double near)
{
	double			ratio;
	double			height;
	double			width;
	t_m4			proj;
	t_v2i			geo;

	glfwGetWindowSize(window, &geo.x, &geo.y);
	ratio = (double)geo.x / (double)geo.y;
	height = near * (tan(fov * 0.5 * (M_PI / 180)));
	width = height * ratio;
	proj = geo_mk4_projection(
		(t_proj){-width, width, -height, height, near, far});
	return (proj);
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

static void			display_pre(GLFWwindow *window, t_vertex_pack *pack)
{
	event_texture_mode(window, &pack->uniforms);
	auto_rotate(pack->input, &pack->object);
}

/*
** main display_loop
*/

int					display_loop(GLFWwindow *window, t_vertex_pack *pack)
{
	const int		faces_total = (int)(pack->object.stats.faces * 3);
	t_m4f			model;
	t_m4f			view;

	matrix_init(pack);
	glUseProgram(pack->program);
	send_uniforms(window, pack);
	while (!glfwWindowShouldClose(window))
	{
		display_pre(window, pack);
		pack->object.transform.matrix = make_matrix(window, pack);
		model = geo_mk4_tof(pack->object.transform.matrix);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(pack->uniforms.model_view, 1, GL_FALSE,
			(const GLfloat *)&model);
		view = geo_mk4_tof(pack->camera);
		glUniformMatrix4fv(pack->uniforms.camera, 1, GL_FALSE,
			(const GLfloat *)&view);
		glBindVertexArray(pack->object.vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pack->object.indices);
		glDrawElements(GL_TRIANGLES, faces_total, GL_UNSIGNED_INT, NULL);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return (0);
}
