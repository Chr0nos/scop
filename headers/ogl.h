/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ogl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/24 07:24:26 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/11 18:43:30 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OGL_H
# define OGL_H
# include "draw.h"
# include "libft.h"
# include <SOIL2.h>
# include <GL/glew.h>
# include <GLFW/glfw3.h>
# define FLAG_UV		1u

typedef struct			s_obj_stats
{
	size_t				fullsize;
	size_t				vertex;
	size_t				uv;
	size_t				faces;
	size_t				points;
	size_t				normal;
}						t_obj_stats;

typedef struct			s_vertex_item
{
	t_v3f				position;
	t_v4f				color;
	t_v2f				uv;
	t_v3f				normal;
}						t_vertex_item;

typedef struct			s_vertex_attribs
{
	GLint				position;
	GLint				color;
	GLint				uv;
	GLint				normal;
}						t_vertex_attribs;

typedef struct			s_vertex_pack
{
	t_vertex_item		*vertex_items;
	t_v3f				*vertex;
	t_v2f				*uv;
	unsigned char		*flags;
	t_v3i				*faces;
	t_v3i				*fuv;
	t_v3f				*normals;
	t_obj_stats			stats;
	t_v3f				center;
	GLuint				fs;
	GLuint				vs;
	GLuint				program;
	GLuint				vbo;
	GLuint				vao;
	GLuint				texture;
	GLuint				indices;
	GLuint				normal;
	GLuint				index_uv;
	t_vertex_attribs	attribs;
	GLint				proj_id;
	GLint				model_id;
	GLint				texture_id;
	const char			*texture_path;
}						t_vertex_pack;

void					fixcenter(t_vertex_pack *pack);
t_vertex_pack			*parse_obj(const char *filepath);
t_obj_stats				parser_count(const char *filepath);
int						keyboard(GLFWwindow *window);
void					framebuffer_size_callback(GLFWwindow *window,
	int width, int height);
void					error_handler(int id, const char *str);
void					vertex_debug(t_printf *pf);
void					faces_debug(t_printf *pf);
t_v3i					*load_faces(t_list *faces, const int max,
	size_t *faces_count);
void					clean_pack(t_vertex_pack *pack);
t_m4					get_projection(GLFWwindow *window, double fov,
	double far, double near);
t_m4					make_matrix(GLFWwindow *window);
size_t					parse_calc_size(const t_obj_stats *stats);
int						display_loop(GLFWwindow *window, t_vertex_pack *pack);
int						run_parse(const char *filepath, const char *texture);
int						parse_face(const char *line, t_vertex_pack *pack);
int						make_program(t_vertex_pack *pack);
int						make_vertex_items(t_vertex_pack *pack);
void					send_attributes(t_vertex_pack *pack);

#endif
