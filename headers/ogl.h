/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ogl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/24 07:24:26 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 04:34:22 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OGL_H
# define OGL_H
# include "draw.h"
# include "libft.h"
# include "quaternion.h"
# include <SOIL2.h>
# include <GL/glew.h>
# include <GLFW/glfw3.h>
# define FLAG_UV		1u
# define FLAG_SW_NONE	0
# define FLAG_SW_IN		1
# define FLAG_SW_OUT	2
# define DISPLAY_FOV	45
# define FLAG_SH_NMAP	(1u << 0)
# define FLAG_SH_NLIGHT	(1u << 1)
# define INPUT_LCLICK	(1u << 0)
# define INPUT_RCLICK	(1u << 1)
# define INPUT_MCLICK	(1u << 2)
# define INPUT_RLAST	(1u << 3)
# define INPUT_CLICK	(INPUT_LCLICK | INPUT_RCLICK)
# define INPUT_AUTOROT	(1u << 4)
# define AXIS_X			(t_v3d){1.0, 0.0, 0.0}
# define AXIS_Y			(t_v3d){0.0, 1.0, 0.0}
# define AXIS_Z			(t_v3d){0.0, 0.0, 1.0}
# define SHADER_FRAGM	"./shaders/fragment.glsl"
# define SHADER_VERTEX	"./shaders/vertex.glsl"
# define DEFAULT_TEXTURE "./ressources/textures/default.jpg"

enum					e_texture_type {
	DIFFUSE = 0,
	NORMAL_MAP,
	AMBIANT_OCCLUSION,
	EMISSION_MAP
};

/*
** id: the uniform id
** opengl_id : image texture id
*/

struct					s_texture_info
{
	const char			*filepath;
	const char			*uniform;
	GLuint				id;
	GLint				opengl_id;
	enum e_texture_type	type;
};

# define TEXTURES_COUNT	3

typedef struct			s_obj_stats
{
	size_t				vertex;
	size_t				uv;
	size_t				faces;
	size_t				points;
	size_t				normal;
	size_t				current_vertex;
	size_t				current_uv;
	size_t				current_normal;
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

typedef struct			s_uniforms
{
	GLint				proj;
	GLint				model_view;
	GLint				texture_switch;
	GLint				texture_switch_mode;
	float				texture_switch_val;
	GLint				light_pos;
	GLint				light_color;
	GLint				camera;
	GLint				flags;
}						t_uniforms;

typedef struct			s_light
{
	t_v3f				position;
	t_v4f				color;
}						t_light;

struct					s_transform {
	t_quaternion		q;
	t_m4				matrix;
	t_v3d				axis;
	t_v3f				scale;
	t_v3f				position;
};

struct					s_object
{
	unsigned int		flags;
	GLuint				vbo;
	GLuint				vao;
	t_vertex_item		*items;
	t_v3i				*faces;
	t_v3i				*fuv;
	t_v3f				*normals;
	t_v3i				*fnormals;
	t_v2f				*uv;
	t_v3f				*vertex;
	t_obj_stats			stats;
	t_v3f				center;
	GLuint				indices;
	GLuint				normal;
	GLuint				index_uv;
	t_vertex_attribs	attribs;
	struct s_transform	transform;
};

typedef struct			s_vertex_pack
{
	struct s_object		object;
	size_t				object_count;
	t_m4				camera;
	t_quaternion		camera_quat;
	t_v2i				mouse_last;
	size_t				input;
	double				fov;
	unsigned int		flags_shader;
	GLuint				fs;
	GLuint				vs;
	GLuint				program;
	struct s_texture_info textures[TEXTURES_COUNT];
	t_uniforms			uniforms;
	t_light				light;
}						t_vertex_pack;

void					auto_rotate(const size_t input, struct s_object *object);
void					configure_opengl(void);
void					error_handler(int id, const char *str);
void					clean_pack(t_vertex_pack *pack);
t_m4					get_projection(GLFWwindow *window, double fov,
	double far, double near);
t_m4					make_matrix(GLFWwindow *window, t_vertex_pack *pack);
int						display_loop(GLFWwindow *window, t_vertex_pack *pack);
int						make_program(t_vertex_pack *pack);
int						make_vertex_items(t_vertex_pack *pack);
void					send_attributes(const GLuint program, struct s_object *object);
t_vertex_pack			*get_pack(t_vertex_pack *pack);
void					send_uniforms(GLFWwindow *window, t_vertex_pack *pack);

void					reset_camera(t_vertex_pack *pack);
void					reset_model(struct s_object *object);
void					light_toggle(t_vertex_pack *pack);
void					flag_toggle(t_vertex_pack *pack,
		const unsigned int flag);
void					matrix_init(t_vertex_pack *pack);
t_m4					make_matrix(GLFWwindow *window, t_vertex_pack *pack);
void					light_move(int key, t_vertex_pack *pack, const float speed);

/*
** events
*/

void					update_projection(GLFWwindow *window,
		t_vertex_pack *pack);
void					mouse_button_callback(GLFWwindow *window, int button,
		int action, int mods);
void					mouse_pos_callback(GLFWwindow *window, double xpos,
		double ypos);
void					scroll_callback(GLFWwindow *window, double xoffset,
		double yoffset);
void					key_callback(GLFWwindow *window, int key, int scancode,
		int action, int mods);
void					framebuffer_size_callback(GLFWwindow *window,
	int width, int height);
void					dragndrop(GLFWwindow *window, int count,
	const char **files);

/*
** parser
*/

int						parse_face(const char *line, struct s_object *object);
int						parse_duplicate(struct s_object *object);
void					color_load(t_v4f *target, const unsigned int color);
int						parse_post_process(struct s_object *object);
int						parse_real(const char *filepath, struct s_object *object);
int						run_parse(int ac, char **av);
int						parse_obj(t_vertex_pack *pack, const char *filepath);
t_obj_stats				parser_count(const char *filepath);
t_v3i					*load_faces(t_list *faces, const int max,
	size_t *faces_count);
void					fixcenter(struct s_object *object);

void					debug_tbn(t_vertex_pack *pack);

#endif
