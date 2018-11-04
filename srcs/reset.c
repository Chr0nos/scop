/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/01 13:57:30 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 04:07:02 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void		reset_camera(t_vertex_pack *pack)
{
	pack->camera_quat = geo_quat_identity();
	pack->camera = geo_quat_tomatrix(pack->camera_quat);
	pack->camera.w.z = -8;
}

void		reset_model(struct s_object *object)
{
	object->transform.q = geo_quat_identity();
	object->transform.matrix = geo_quat_tomatrix(object->transform.q);
}
