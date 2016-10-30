/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ogl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/24 07:24:26 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/30 15:02:39 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OGL_H
# define OGL_H
# include "draw.h"
# include "libft.h"
# include <SOIL2.h>
# include <GLFW/glfw3.h>
# define FLAG_UV		1u

typedef struct			s_vertex_pack
{
	t_v3f				*vertex;
	t_v2f				*uv;
	unsigned char		*flags;
	size_t				points;
	t_v3i				*faces;
	size_t				faces_count;
	t_v3f				center;
}						t_vertex_pack;

int						keyboard(GLFWwindow *window);
void					framebuffer_size_callback(GLFWwindow *window,
	int width, int height);

t_vertex_pack			*load_obj(const char *filepath);
void					display(const GLuint texture, t_vertex_pack *pack,
	GLFWwindow *window);
void					error_handler(int id, const char *str);
void					vertex_debug(t_printf *pf);
void					faces_debug(t_printf *pf);
t_v3i					*load_faces(t_list *faces, const int max,
	size_t *faces_count);
void					clean_pack(t_vertex_pack *pack);
void				load_projection(double ratio, double fov, double far,
	double near);

#endif
