/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scroll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/01 14:54:57 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/01 15:00:17 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void	scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	t_vertex_pack	*pack;

	(void)window;
	(void)xoffset;
	pack = get_pack(NULL);
	pack->camera.w.z += yoffset * 0.5;
}
