/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maker.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 12:35:02 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/24 00:54:13 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "opengl.h"

static void			make_vertex_items_uv(t_vertex_pack *pack, const size_t p)
{
	t_vertex_item		*item;
	const int			max_vertex = (int)pack->stats.vertex;
	const int			max_faces = (int)pack->stats.faces;

	if (p > pack->stats.faces)
		return ;
	if ((pack->faces[p].x >= max_faces) || (pack->fuv[p].x >= max_vertex) ||
		(pack->faces[p].y >= max_faces) || (pack->fuv[p].y >= max_vertex) ||
		(pack->faces[p].z >= max_faces) || (pack->fuv[p].z >= max_vertex))
		return ;
	item = &pack->vertex_items[pack->faces[p].x];
	item->uv = pack->uv[pack->fuv[p].x];
	item = &pack->vertex_items[pack->faces[p].y];
	item->uv = pack->uv[pack->fuv[p].y];
	item = &pack->vertex_items[pack->faces[p].z];
	item->uv = pack->uv[pack->fuv[p].z];
}

int					make_vertex_items(t_vertex_pack *pack)
{
	t_vertex_item		*item;
	size_t				p;

	p = 0;
	while (p < pack->stats.vertex)
	{
		item = &pack->vertex_items[p];
		item->position = pack->vertex[p];
		item->uv = pack->uv[p];
		p++;
	}
	p = pack->stats.faces;
	ft_printf("patching %lu faces\n", p);
	while (p--)
		make_vertex_items_uv(pack, p);
	return (0);
}

static int			make_vao(t_vertex_pack *pack)
{
	make_vertex_items(pack);
	ft_putendl("making vbo");
	pack->vbo = 0;
	glGenBuffers(1, &pack->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vbo);
	glBufferData(GL_ARRAY_BUFFER,
		(GLsizeiptr)(sizeof(t_vertex_item) * pack->stats.vertex),
		(float*)pack->vertex_items,
		GL_STATIC_DRAW);
	ft_putendl("making faces indices");
	ft_opengl_buffer_load(&pack->indices, GL_ELEMENT_ARRAY_BUFFER,
		pack->faces, pack->stats.faces * sizeof(t_v3i));
	ft_putendl("making vao");
	pack->vao = 0;
	glGenVertexArrays(1, &pack->vao);
	glBindVertexArray(pack->vao);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vbo);
	send_attributes(pack);
	glBindVertexArray(0);
	return (0);
}

static int			make_texture(t_vertex_pack *pack)
{
	ft_printf("loading texture: %s\n", pack->texture_path);
	glEnable(GL_TEXTURE_2D);
	pack->texture = SOIL_load_OGL_texture(
		pack->texture_path,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, pack->texture);
	glActiveTexture(GL_TEXTURE0);
	pack->texture_id = glGetUniformLocation(pack->program, "texture");
	glEnableVertexAttribArray((GLuint)pack->texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(pack->texture_id, (GLint)pack->texture);
	return (0);
}

int					make_program(t_vertex_pack *pack)
{
	ft_putendl("making program");
	if (!(pack->fs = ft_shader_compile(GL_FRAGMENT_SHADER, "fragment.glsl")))
		return (1);
	if (!(pack->vs = ft_shader_compile(GL_VERTEX_SHADER, "vertex.glsl")))
		return (2);
	ft_putendl("shaders ok");
	pack->program = glCreateProgram();
	glBindAttribLocation(pack->program, 0, "my_position");
	glBindAttribLocation(pack->program, 1, "my_color");
	glBindAttribLocation(pack->program, 2, "my_uv");
	glBindAttribLocation(pack->program, 3, "my_normal");
	glAttachShader(pack->program, pack->fs);
	glAttachShader(pack->program, pack->vs);
	glLinkProgram(pack->program);
	make_texture(pack);
	make_vao(pack);
	ft_putendl("program done");
	return (0);
}
