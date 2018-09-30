/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attributes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 18:43:49 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/30 17:22:49 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static void		set_attributes(const GLuint program,
	struct s_vertex_attribs *attrs)
{
	*attrs = (t_vertex_attribs) {
		.position = glGetAttribLocation(program, "my_position"),
		.color = glGetAttribLocation(program, "my_color"),
		.uv = glGetAttribLocation(program, "my_uv"),
		.normal = glGetAttribLocation(program, "my_normal"),
		.tangeant = glGetAttribLocation(program, "my_tangeant")
	};
}

void			send_attributes(const GLuint program, struct s_object *object)
{
	const GLsizei			step = sizeof(t_vertex_item);
	const size_t			offset_uv = (object->stats.uv > 0) ? 7 : 0;
	t_vertex_attribs		*attrs;

	attrs = &object->attribs;
	set_attributes(program, attrs);
	ft_printf("%d %d %d %d %d\n", attrs->position,
		attrs->color, attrs->uv, attrs->normal, attrs->tangeant);
	glEnableVertexAttribArray((GLuint)attrs->position);
	glEnableVertexAttribArray((GLuint)attrs->color);
	glEnableVertexAttribArray((GLuint)attrs->uv);
	glEnableVertexAttribArray((GLuint)attrs->normal);
	glEnableVertexAttribArray((GLuint)attrs->tangeant);
	glVertexAttribPointer((GLuint)attrs->position,
		3, GL_FLOAT, GL_FALSE, step, NULL);
	glVertexAttribPointer((GLuint)attrs->color,
		4, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * 3));
	glVertexAttribPointer((GLuint)attrs->uv,
		2, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * offset_uv));
	glVertexAttribPointer((GLuint)attrs->normal,
		3, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * 9));
	glVertexAttribPointer((GLuint)attrs->tangeant,
		3, GL_FLOAT, GL_FALSE, step, (void*)(sizeof(float) * 12));
}
