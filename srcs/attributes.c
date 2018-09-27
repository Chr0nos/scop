/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attributes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 18:43:49 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 04:28:45 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static void		set_attributes(const GLuint program,
	struct s_vertex_attribs *attrs)
{
	attrs->position = glGetAttribLocation(program, "my_position");
	attrs->color = glGetAttribLocation(program, "my_color");
	attrs->uv = glGetAttribLocation(program, "my_uv");
	attrs->normal = glGetAttribLocation(program, "my_normal");
}

void			send_attributes(const GLuint program, struct s_object *object)
{
	const GLsizei		step = sizeof(t_vertex_item);
	const size_t		offset_uv = (object->stats.uv > 0) ? 7 : 0;

	set_attributes(program, &object->attribs);
	ft_printf("%d %d %d %d\n", object->attribs.position,
		object->attribs.color, object->attribs.uv, object->attribs.normal);
	glEnableVertexAttribArray((GLuint)object->attribs.position);
	glEnableVertexAttribArray((GLuint)object->attribs.color);
	glEnableVertexAttribArray((GLuint)object->attribs.uv);
	glEnableVertexAttribArray((GLuint)object->attribs.normal);
	glVertexAttribPointer((GLuint)object->attribs.position,
		3, GL_FLOAT, GL_FALSE, step, NULL);
	glVertexAttribPointer((GLuint)object->attribs.color,
		4, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * 3));
	glVertexAttribPointer((GLuint)object->attribs.uv,
		2, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * offset_uv));
	glVertexAttribPointer((GLuint)object->attribs.normal,
		3, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * 9));
}
