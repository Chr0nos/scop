/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/01 13:57:30 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/01 14:01:00 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void		reset_camera(t_vertex_pack *pack)
{
	pack->camera_quat = geo_quat_identity();
	pack->camera = geo_quat_tomatrix(pack->camera_quat);
}

void		reset_model(t_vertex_pack *pack)
{
	pack->model_quat = geo_quat_identity();
	pack->model = geo_quat_tomatrix(pack->model_quat);
}
