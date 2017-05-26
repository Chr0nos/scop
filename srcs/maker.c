/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maker.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 12:35:02 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/26 22:29:12 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "opengl.h"

static int			make_vao(t_vertex_pack *pack)
{
	ft_putendl("making vbo");
	pack->vbo = 0;
	glGenBuffers(1, &pack->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vbo);
	glBufferData(GL_ARRAY_BUFFER,
		(GLsizeiptr)(sizeof(t_vertex_item) * pack->stats.vertex),
		(float*)pack->items,
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

static void			make_normal_map(t_vertex_pack *pack)
{
	ft_printf("loading normal map at: %s\n", pack->normal_map_path);
	pack->normal_map = SOIL_load_OGL_texture(pack->normal_map_path,
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, pack->normal_map);
	glActiveTexture(GL_TEXTURE0 + 1);
	pack->normal_map_id = glGetUniformLocation(pack->program, "normal_map");
	ft_printf("normal map id: %d %d\n", pack->normal_map, pack->normal_map_id);
	glEnableVertexAttribArray((GLuint)pack->normal_map_id);
	glUniform1i(pack->normal_map_id, (GLint)pack->normal_map);
	glGenerateMipmap(GL_TEXTURE_2D);
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
	pack->texture_id = glGetUniformLocation(pack->program, "texture_sampler");
	ft_printf("texture id: %d %d\n", pack->texture, pack->texture_id);
	glEnableVertexAttribArray((GLuint)pack->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glUniform1i(pack->texture_id, (GLint)pack->texture_id);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (pack->normal_map_path)
		make_normal_map(pack);
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
