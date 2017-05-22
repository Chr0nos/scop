/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attributes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 18:43:49 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/22 16:34:34 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void			send_attributes(t_vertex_pack *pack)
{
	const GLsizei		step = sizeof(t_vertex_item);
	const size_t		offset_uv = (pack->stats.uv > 0) ? 7 : 0;

	pack->attribs.position = glGetAttribLocation(pack->program, "my_position");
	pack->attribs.color = glGetAttribLocation(pack->program, "my_color");
	pack->attribs.uv = glGetAttribLocation(pack->program, "my_uv");
	pack->attribs.normal = glGetAttribLocation(pack->program, "my_normal");
	ft_printf("%d %d %d %d\n", pack->attribs.position, pack->attribs.color,
		pack->attribs.uv, pack->attribs.normal);
	glEnableVertexAttribArray((GLuint)pack->attribs.position);
	glEnableVertexAttribArray((GLuint)pack->attribs.color);
	glEnableVertexAttribArray((GLuint)pack->attribs.uv);
	glEnableVertexAttribArray((GLuint)pack->attribs.normal);
	glVertexAttribPointer((GLuint)pack->attribs.position,
		3, GL_FLOAT, GL_FALSE, step, NULL);
	glVertexAttribPointer((GLuint)pack->attribs.color,
		4, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * 3));
	glVertexAttribPointer((GLuint)pack->attribs.uv,
		2, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * offset_uv));
	glVertexAttribPointer((GLuint)pack->attribs.normal,
		3, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * 9));
}
