/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 15:25:29 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/05 15:26:31 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void			update_projection(GLFWwindow *window, t_vertex_pack *pack)
{
	t_m4f	proj;

	proj = geo_mk4_tof(get_projection(window, pack->fov, 1.0, 1000.0));
	glUniformMatrix4fv(pack->uniforms.proj, 1, GL_FALSE, (const float *)&proj);
}

void			framebuffer_size_callback(GLFWwindow *window,
	int width, int height)
{
	(void)width;
	(void)height;
	update_projection(window, &g_pack);
	glViewport(0, 0, width, height);
}
