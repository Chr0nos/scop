/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/03 13:54:08 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/03 13:56:55 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void		light_toggle(t_vertex_pack *pack)
{
	if (pack->flags_shader & FLAG_SH_NLIGHT)
		pack->flags_shader &= ~FLAG_SH_NLIGHT;
	else
		pack->flags_shader |= FLAG_SH_NLIGHT;
	glUniform1ui(pack->uniforms.flags, pack->flags_shader);
}
