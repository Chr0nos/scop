/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/03 13:54:08 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/03 22:26:17 by snicolet         ###   ########.fr       */
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

void		flag_toggle(t_vertex_pack *pack, const unsigned int flag)
{
	if (pack->flags_shader & flag)
		pack->flags_shader &= ~flag;
	else
		pack->flags_shader |= flag;
	glUniform1ui(pack->uniforms.flags, pack->flags_shader);
	ft_printf("new flags state: %b\n", pack->flags_shader);
}
